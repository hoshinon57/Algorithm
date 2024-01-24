#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC284 https://atcoder.jp/contests/abc284

/*
 * ローリングハッシュで解く。
 * 
 * 入力文字列Tに対して、
 *   T[0,i) + T[N+i, 2N) = reverse(T[i,N+i))
 * となるようなiが存在すればよい。
 * 
 * 左辺はローリングハッシュのライブラリにて合成するメソッドを作成すればよい。
 * [N+i,2N)の長さがN-iなので、T[0,i)^p[N-i] + T[N+i,2N) で求められる。
 * 
 * 右辺はTの反転文字列をrevTとすると、 revT[N-i,2N-i) をローリングハッシュで求めればよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・入力文字列Tについて、
 *     先頭i文字 + 末尾N-i文字
 *     i+1文字目からN文字ぶん、の反転
 *   が一致するかの判定問題となる。
 *   であれば、反転用のハッシュも用意することで、ローリングハッシュで各iについてO(1)で判定できる。
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

	// 先頭idx文字と、末尾のN-idx文字を合成
	// 今回のABC284-F用に作成
	uint64_t mix(vector<uint64_t> &hash, int idx, int N)
	{
		auto h = query(hash, 0, idx);  // 先頭idx文字
		h = (__uint128_t)h * p[N-idx] % mod;
		h += query(hash, idx+N, N*2);  // 末尾N-idx文字
		h %= mod;
		return h;
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
	int i;
	ll N; cin >> N;
	string s; cin >> s;
	auto s_rev = s;
	reverse(s_rev.begin(), s_rev.end());
	RollingHash rh;
	auto hash = rh.build(s);
	auto hash_rev = rh.build(s_rev);

	for(i = 0; i <= N; i++)
	{
		auto h = rh.mix(hash, i, N);
		auto h_rev = rh.query(hash_rev, N-i, 2*N-i);
		if(h == h_rev)  // 一致した
		{
			string ans = s.substr(0, i);  // 先頭i文字
			ans += s.substr(s.size() - (N-i));  // 末尾から(N-i)文字取り出し
			cout << ans << endl << i << endl;
			return 0;
		}
	}
	cout << -1 << endl;

	return 0;
}
