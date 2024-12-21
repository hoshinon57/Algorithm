#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// AOJ 2876
// https://onlinejudge.u-aizu.ac.jp/challenges/sources/VPC/RUPC/2876

/*
 * 集合の一致なのでZobrist Hashを用いる。
 * ＞1.集合の要素にランダムな整数を割り当てる。
 * ＞2.集合のハッシュは割り当てられた整数をxorしたもの。
 * Aiが最大で10^9と大きいので、各値に事前にランダム値を割り当てるのは間に合わない。
 * mapを用いてランダム値を用意する。rnd()を参照。
 * 
 * 同じ値が集合に含まれる(多重集合)のため、xorではなく加算してハッシュとする。
 * ＞多重集合の場合、割り当てられた整数を偶数回xorすると0になってしまい、区別がつかない。
 * ＞xorではなく加算することでハッシュにできる。かんたん。
 */

// [lo,hi]の区間の乱数を返す(閉区間)
// 必要なincludeは以下2つ
//   #include <random>
//   #include <chrono>
// 型はlong longで決め打ちしている。(mt19937_64を使っており、生成される値は64bit)
// 参考：
//   https://betrue12.hateblo.jp/entry/2019/09/07/171628
//   https://zenn.dev/mafafa/articles/f1030c3a014d4e
//   https://ei1333.github.io/luzhiled/snippets/other/random-number-generator.html
//   https://yosupo.hatenablog.com/entry/2024/06/14/064913
//   https://rsk0315.hatenablog.com/entry/2020/09/02/164428
//   https://cpprefjp.github.io/reference/random/mt19937_64.html
//   https://cpprefjp.github.io/reference/random/uniform_int_distribution.html
long long rng_64(long long lo, long long hi) {
	// static std::random_device rd;  // random_deviceを使うのは避けた方が良い環境があるらしい
	// static std::mt19937_64 rng(rd());
	static auto seed = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	static std::mt19937_64 rng(seed);
	return std::uniform_int_distribution<long long>(lo, hi)(rng);  // [lo,hi]の範囲での乱数を生成
}

// 値nに対する乱数を返す
ll rng(ll n)
{
	static map<ll,ll> rng;
	if(rng.count(n) == 0) rng[n] = rng_64(0, INF64);  // 値nに対する乱数を初回生成

	return rng[n];
}

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> sum(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {sum[kk+1] = sum[kk] + rng(a[kk]);}

	vector<int> ans;
	for(int k = 0; k < N; k++)  // [0,k] と [N-k-1,N-1]  -> [0,k+1) と [N-k-1,N)
	{
		ll tmp1 = sum[k+1] - sum[0];
		ll tmp2 = sum[N] - sum[N-k-1];
		if(tmp1 == tmp2) ans.push_back(k+1);
	}

	int sz_ = (int)ans.size();
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
