#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <array>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC339 https://atcoder.jp/contests/abc339

/*
 * ABC339-F.cppの実行時間を改善した版。
 * いくつか試してみたが、複数ハッシュをvectorでまとめた上で
 *   vector -> array に変更
 * するのが最も改善された。
 * 
 * 3つのハッシュを array<uint64_t, 3> m; でまとめ、mをキーとしてmapに保存している。
 * 
 * ABC339-F.cppでは2種ハッシュで1026msだったが、
 * ABC339-F-2.cppでは3種で698msだった。
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
	RollingHash rh1, rh2(8005397), rh3(10014757);  // 8005397,10014757:適当な素数
	map<array<uint64_t, 3>, ll> mp;  // ハッシュ登場回数
	using hash = vector<uint64_t>;
	vector<hash> hashes1(N), hashes2(N), hashes3(N);
	for(i = 0; i < N; i++)
	{
		array<uint64_t, 3> m;
		hashes1[i] = rh1.build(n[i]);
		m[0] = rh1.query(hashes1[i]);

		hashes2[i] = rh2.build(n[i]);
		m[1] = rh2.query(hashes2[i]);

		hashes3[i] = rh3.build(n[i]);
		m[2] = rh3.query(hashes3[i]);

		mp[m]++;
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			auto d1 = rh1.mul(rh1.query(hashes1[i]), rh1.query(hashes1[j]));
			auto d2 = rh2.mul(rh2.query(hashes2[i]), rh2.query(hashes2[j]));
			auto d3 = rh3.mul(rh3.query(hashes3[i]), rh3.query(hashes3[j]));
			array<uint64_t, 3> m = {d1, d2, d3};
			ans += mp[m];
		}
	}
	cout << ans << endl;

	return 0;
}
