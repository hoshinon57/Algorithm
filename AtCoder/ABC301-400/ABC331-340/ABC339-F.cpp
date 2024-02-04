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

// ABC339 https://atcoder.jp/contests/abc339

/*
 * コンテスト中に解けず。(方針は悪くなかったが、ハッシュ衝突の認識が甘かった)
 * ローリングハッシュを用いて解いた。
 * 
 * 公式解説(https://atcoder.jp/contests/abc339/editorial/9206)の通りで、
 * ある適当な値xを用いて
 *   Ai*Aj≡Ak (mod x)
 * となれば、元の式Ai*Aj=Akも成立すると判断してしまうこととする。
 * 自作のローリングハッシュのライブラリにて、基数を10に固定させることで mod x における値を計算した。
 * 
 * ただしmodは (1<<61)-1 を使っていたが、これについてハッシュ衝突させるテストケースが存在し、コンテスト時はWAだった。
 * よって複数modでのハッシュを用意して判定してACとなった。
 * これは連想配列を用いて各Aについて
 *   mp[(ハッシュ値)] = (そのハッシュの登場回数)
 * としておき、
 *   mp[(Aiのハッシュ値)*(Ajのハッシュ値)]
 * を2種のmodについてそれぞれ計算する。
 * ハッシュの衝突をふまえると、上記2値のminが求めたい値となる。
 * 
 * ただし自作ライブラリの定数倍が悪いのか、そもそも本問でローリングハッシュのライブラリを使うのはやりすぎなのか、
 * 2種modで実行時間が1秒以上かかる結果だった。
 * 公式解説など、処理時間が短い実装も調べておきたい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・ローリングハッシュを用いる点までは思いついた。
 * ・WAになったとき、ハッシュ衝突を疑う。
 *   本問では Ak=x, x+mod でハッシュ衝突してしまうので、非有名なmodを用意するか、複数使うべきだった。
 */

class RollingHash
{
	const uint64_t mod;  // デフォルトは (1ull << 61) -1;
//	const uint64_t base = 1897230259;  // とりあえず基数決め打ち(素数)
	const uint64_t base = 10;
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

	uint64_t mul(uint64_t h1, uint64_t h2)
	{
		return (uint64_t)((__uint128_t)h1 * h2 % mod);
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
	ll i, j;
	ll N; cin >> N;
	vector<vector<int>> n(N);
	vector<string> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		for(j = 0; j < (int)a[i].size(); j++)
		{
			n[i].push_back(a[i][j]-'0');
		}
	}
	RollingHash rh1, rh2(8005397);  // 8005397:適当な素数
	map<uint64_t, ll> mp1, mp2;  // ハッシュ登場回数
	using hash = vector<uint64_t>;
	vector<hash> hashes1(N), hashes2(N);
	for(i = 0; i < N; i++)
	{
		hashes1[i] = rh1.build(n[i]);
		mp1[rh1.query(hashes1[i])]++;

		hashes2[i] = rh2.build(n[i]);
		mp2[rh2.query(hashes2[i])]++;
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			auto d1 = rh1.mul(rh1.query(hashes1[i]), rh1.query(hashes1[j]));
			auto d2 = rh2.mul(rh2.query(hashes2[i]), rh2.query(hashes2[j]));
			// ハッシュ衝突を踏まえると、それぞれの登場回数のうち小さい方が答
			ans += min(mp1[d1], mp2[d2]);
		}
	}
	cout << ans << endl;

	return 0;
}
