#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC377 https://atcoder.jp/contests/abc377

/*
 * 自力で解けず、解説を見た。
 * 公式解説とは異なるが、ローリングハッシュを用いて解く。
 *   https://atcoder.jp/contests/abc377/editorial/11249
 * 
 * 入力Sに対し、先頭からi文字までの連続部分文字列S'を考え、
 *   map[S']=|S|
 * となるようなmapをSの各iについて求めることを考える。
 * 
 * Sについての答は、Sの先頭j文字まで残し、他の文字列を加える操作を考えて、
 *   (|S|-j) + (map[S']-j)
 * を各jについて計算した最小値となる。
 * ※いらないゴミを削り、他のSになるよう追加していくイメージ。
 * 
 * ただしこれを単純な map<string,int> でやるとTLE.
 * |S|=2*10^5 である文字列から連続部分文字列を作ると文字列長の総和は O(|S|^2) のオーダーになり、
 * mapでのハッシュ計算が間に合わないと思われる。（実際にTLEした）
 * そこで、mapには文字列ではなくローリングハッシュのハッシュ値を用いるようにすると、間に合う。
 * 
 * ※自作のローリングハッシュライブラリが、Query()にて空文字列は選べない点に注意。
 *   ansの初期値を|S|にして対策した。
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
	const uint64_t mod;  // デフォルトは (1ull << 61) -1;
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
	RollingHash(uint64_t mod_ = (1ull << 61) -1) : mod(mod_), p{1} {;}

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
	int i, j;
	int N; cin >> N;
	RollingHash rh;
	map<uint64_t, int> hmap;  // hmap[val]=(先頭からの連続部分文字列のハッシュがvalのうち、|S|が最小のもの)
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		auto hash = rh.build(s);
		int len = (int)s.size();
		int ans = len;  // 空文字にするケース
		for(j = 1; j <= len; j++)  // sの先頭j文字まで残す
		{
			auto q = rh.query(hash, 0, j);  // [0,j)のハッシュq
			if(hmap.count(q) != 0)  // sの先頭j文字が、すでに存在する場合
			{
				int cnt = (len-j) + (hmap[q]-j);  // 入力Sを削るコストと、そこから他のSになるよう文字を足していくコスト、の和
				chmin(ans, cnt);
				chmin(hmap[q], len);
			}
			else
			{
				hmap[q] = len;
			}
		}
		cout << ans << endl;
	}

	return 0;
}
