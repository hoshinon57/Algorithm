#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC141 https://atcoder.jp/contests/abc141

/*
 * ローリングハッシュ＆二分探索で解く。
 * 最初、自力ACしたが計算量が悪くTLEに苦戦した。以下の解説を参考に実装しなおした。
 *   https://maspypy.com/atcoder-%E5%8F%82%E5%8A%A0%E6%84%9F%E6%83%B3-2019-09-14abc-141
 *   https://betrue12.hateblo.jp/entry/2019/09/16/013915
 * 結果、当初950ms⇒再実装11msと、大幅に改善した。
 * 
 * 答の文字列長をlenとすると、len-1,len-2,... も「最長」以外の条件を満たす。
 * よって二分探索で境界を求めることができる。
 * 
 * 判定問題は「長さlenが条件を満たすか」となる。
 * mapを用いて mp[hash値]=(文字列の位置) とすると、Sを左から1文字ずつ見ていって、
 *   len文字分のハッシュ値がmpに未登場なら、その位置を登録
 *   ハッシュ値が登録済で、かつその位置がlen以上離れていればOK.
 *   最後までOKにならなければNG.
 * となる。
 * 
 * 計算量は、二分探索がO(logN), 判定問題がO(N*logN)となり(※)、全体でO(N*(logN)^2).
 * (※)ループがO(N), map操作がO(logN).
 * 
 * [ACまでの思考の流れ / どう思考すれば解法にたどり着けるか]
 * ・ローリングハッシュの例題として載っていたので、それを使うのは分かっていた。
 * ・「重ならずに」という条件をどうするかと考えたときに、
 *   mapで [ハッシュ値]=(文字列の位置) とすれば、同じハッシュ値が登場したときに離れている文字数が分かる。
 * ・ただここから二分探索に思い至らず、lenを1～Nまで順に増やしていく実装にしてしまった。
 *   lenが条件を満たすならlen-1も満たすので、単調性により二分探索が使える。
 */

/*
 * ローリングハッシュのライブラリ
 * [使い方]
 * ・RollingHash rh に対し、rh.build(str) で文字列strのハッシュを返す。
 *   ハッシュは vector<uint64_t> なのでautoで受けることを推奨。
 *   配列vのハッシュを返すbuild(v)もある。ただし値が0の要素が含まれないように注意。
 * ・rh.query()で、[l,r)のハッシュ値を返す。 全体[0,size)のハッシュ値を返す版もある。
 *   ハッシュが等しければ(高い確率で)文字列が等しいと判断できる。
 * ・rh.lcp()で、2つの区間のLCP(最長共通接頭辞, 一致する文字数の最大)の長さを返す。
 *   何文字一致しているかを調べるのに便利。
 *   二分探索で実装している。
 * 
 * [ToDo]
 * ・modを(2^61-1)としており、計算途中で雑に __uint128_t でキャストしている メンバ関数にmul()を用意するなどして改善したい
 * ・基数baseを決め打ちにしており、ランダム生成に変える
 */
class RollingHash
{
	const uint64_t mod = (1ull << 61) -1;
	const uint64_t base = 1897230259;  // とりあえず基数決め打ち(素数)
	vector<uint64_t> p;

	void expand(size_t sz)
	{
		if(p.size() < sz + 1)
		{
            int pre_sz = (int)p.size();
            p.resize(sz+1);
            for(int i = pre_sz-1; i < (int)sz; i++)
			{
//				p[i+1] = (p[i]*base)%mod;
				p[i+1] = ((__uint128_t)p[i]*base)%mod;
            }
		}
	}

public:
	RollingHash() : p{1} {;}

	// 文字列sのハッシュを計算して返す
	vector<uint64_t> build(string &s)
	{
		size_t size = s.size();
		vector<uint64_t> hash(size + 1);  // hash[0]=0
        // size=3 の場合は
        // [0,0) [0,1) [0,2) [0,3)
		expand(size);
		for(size_t i = 0; i < size; i++)
		{
//			hash[i+1] = hash[i] * base + s[i];
			hash[i+1] = (__uint128_t)hash[i] * base % mod + s[i];
			hash[i+1] %= mod;
		}
		return hash;
	}

	// 配列vのハッシュを計算して返す
	// v[]に値が0の要素が含まれるのはNGのはず(ハッシュ衝突) どの要素も1以上であること
	vector<uint64_t> build(vector<int> &v)
	{
		size_t size = v.size();
		vector<uint64_t> hash(size + 1);  // hash[0]=0
		expand(size);
		for(size_t i = 0; i < size; i++)
		{
			hash[i+1] = (__uint128_t)hash[i] * base % mod + v[i];
			hash[i+1] %= mod;
		}
		return hash;
	}

	// hashの[l,r)のハッシュ値を返す
	uint64_t query(vector<uint64_t> &hash, int l, int r)
	{
		expand(r-l);
		uint64_t ret = hash[r] + mod - ((__uint128_t)hash[l] * p[r-l] % mod);
		ret %= mod;
		return ret;
	}

	// hashの[0,size) 全体のハッシュ値を返す
	uint64_t query(vector<uint64_t> &hash)
	{
		return hash.back();
	}

	// hash1の区間[l1,r1)とhash2の区間[l2,r2)のlcp(最長共通接頭辞, 一致する文字数の最大)の長さを返す
	int lcp(vector<uint64_t> &hash1, int l1, int r1, vector<uint64_t> &hash2, int l2, int r2)
	{
		int len = min(r1-l1, r2-l2);
		int ok = 0;
		int ng = len+1;
		auto check = [&](int m) -> bool
		{
			return (query(hash1, l1, l1+m) == query(hash2, l2, l2+m));
		};
		while(ng - ok > 1)
		{
			int mid = (ok+ng)/2;
			if(check(mid)) ok = mid;
			else ng = mid;
		}
		return ok;
	}
};

int main(void)
{
	int N; cin >> N;
	string s; cin >> s;
	RollingHash rh;
	auto hash = rh.build(s);

	// 長さlenが条件を満たすか判定
	auto check = [&](int len) -> bool
	{
		map<uint64_t, int> mp;  // (hash,pos)
		for(int k = 0; k <= N-len; k++)  // [k,k+len)
		{
			auto q = rh.query(hash, k, k+len);
			if(mp.count(q))  // 同じ文字列がすでにあれば、len以上離れているか判定
			{
				if(mp[q]+len <= k) return true;
			}
			else mp[q] = k;  // 新たな文字列ならmapに追加
		}
		return false;  // 最後まで見つからなかった
	};

	int l = 0, r = N/2+1;
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}
	cout << l << endl;

	return 0;
}
