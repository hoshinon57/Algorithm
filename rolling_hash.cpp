#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

// ローリングハッシュのライブラリ

//次やること
//ソース整理 include内容とか、コメントとか、solveをsolve_AOJ_ALDS1_14_Bに変えるとか

/*
 * [参考URL]
 *   https://ei1333.github.io/library/string/rolling-hash.hpp  ほぼこのままの実装
 *   https://kyoroid.github.io/algorithm/string/rolling_hash.html
 *   https://qiita.com/hirominn/items/80464ee381c8d400725f
 *   https://trap.jp/post/1036/
 *   https://tjkendev.github.io/procon-library/python/string/rolling_hash.html
 * 
 * [ざっくり概要]
 * 文字列や数列のハッシュを計算することで、要素同士の比較をO(1)で可能とする。
 * 
 * [関連する問題 / verifyした問題]
 * AOJ ALDS1_14_B https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B&lang=ja
 * ABC141-E
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

// 以下はAOJ ALDS1_14_Bを解く内容
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B&lang=ja
void solve(void)
{
	string t, p; cin >> t >> p;
	RollingHash rh;
	int len1 = (int)t.size();
	int len2 = (int)p.size();
	auto h1 = rh.build(t);
	auto h2 = rh.build(p);
	auto q2 = rh.query(h2);
	for(int i = 0; i+len2 <= len1; i++)
	{
		auto q1 = rh.query(h1, i, i+len2);
		if(q1 == q2)
		{
			cout << i << endl;
		}
	}
}

void solve_ABC141_E(void)
{
	int i, j;
	int N; cin >> N;
	string s; cin >> s;
	RollingHash rh;
#if 0
	vector<int> v;
	for(auto &e : s)
	{
		v.push_back(e-'a'+1);
	}
	vector<uint64_t> hash = rh.build(v);
#else
	vector<uint64_t> hash = rh.build(s);
#endif

	int ans = 0;
	for(i = 1; i <= N/2; i++)  // 長さi
	{
//		set<pair<uint64_t, int>> list;  // {hash, pos}
		vector<pair<uint64_t, int>> list;
		for(j = 0; j+i <= N; j++)  // 位置jスタート
		{
			auto h = rh.query(hash, j, j+i);
//			list.insert({h, j});
			list.push_back({h,j});
			/*
			auto itr = list.lower_bound({h, 0});
			int pos = itr->second;
			if(pos + i <= j)
			{
				ans = i;
				break;
			}
			*/
		}
		sort(list.begin(), list.end());
		for(j = 0; j < (int)list.size(); j++)
		{
			auto itr = lower_bound(list.begin(), list.end(), make_pair(list[j].first, list[j].second+i));
			if(itr == list.end()) continue;
			if(itr->first != list[j].first) continue;
			ans = i;
			break;
		}
		/*
		for(j = 0; j <(int)list.size()-1; j++)
		{
			if((list[j].first == list[j+1].first) 
				&& list[j].second+i <= list[j+1].second)
			{
				ans = i;
				break;
			}
		}
		*/
	}
	cout << ans << endl;
}

int main(void)
{
	{
	    string str1 = "abcdefghijklmnabcdefghijklmnabcdefghijklmn";
	    string str2 = "bcdefzzz";
		RollingHash rh;
		auto hash1 = rh.build(str1);
		auto hash2 = rh.build(str2);
		assert(rh.lcp(hash1, 1, str1.size(), hash2, 0, str2.size()) == 5);
	}
	solve_ABC141_E();
	return 0;
	
	solve();
	return 0;

	using hash_type = vector<uint64_t>;
	RollingHash rh;
    string str1 = "abcdefghijklmnabcdefghijklmnabcdefghijklmn";
    string str2 = "mn";
	int len1 = (int)str1.size();
	int len2 = (int)str2.size();
	hash_type h1 = rh.build(str1);
	hash_type h2 = rh.build(str2);
	int i;
	for(i = 0; i+len2 <= len1; i++)
	{
		auto tmp1 = rh.query(h1, i, i+len2);
//		auto tmp2 = rh.query(h2, 0, len2);
		auto tmp2 = rh.query(h2);
		if(tmp1 == tmp2)
		{
			cout << "find:" << i << endl;
		}
	}

	return 0;
}
