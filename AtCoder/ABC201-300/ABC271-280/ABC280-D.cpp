#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC280 https://atcoder.jp/contests/abc280

/*
 * 素因数分解を用いて解く。
 * 
 * まず、Kを素因数分解して、p^e*p^e*... の形にする。
 * 例えばp^eを2^5としたときに、
 *   2:2で1回割れる
 *   4:2で2回割れる
 *   6:2で1回割れる
 *   8:2で3回割れる  ここで計5回以上割れる
 * ことから、N=8以上であれば2^5について条件を満たす。
 * これを他のp^eについても求め、その最大値が答。
 * 
 * 1つのp^eに対して計算量はO(e)となるはず。
 * K=10^12より、2^40ぐらいが上限。よってこの部分は十分に高速。
 * 全体の計算量としては素因数分解が効いてきてO(√K)かと。
 */

// Nを素因数分解し、{素因数, 指数}のpairの形でprime_listに入れる
// 以下URLをほぼそのまま持ってきている
// https://qiita.com/drken/items/a14e9af0ca2d857dad23
// その他、参考URL：
//   https://algo-method.com/descriptions/119
void prime_factorize(long long N, vector<pair<long long, long long>> &prime_list)
{
	long long i;
	// 入力Nに対して、2から√Nの各整数で割れるだけ割っていく
	for(i = 2; i*i <= N; i++)
	{
		if(N%i != 0) continue;  // iが素因数でなければ次へ

		long long ex = 0;  // 指数
		while(N%i == 0)  // 割れる限り割る
		{
			N /= i;
			ex++;
		}
		prime_list.push_back({i, ex});  // iで割り切ったら結果を保存
	}
	if(N != 1)  // 割り切った後に1以外の値で残っていれば、それも素因数
	{
		prime_list.push_back({N, 1});
	}

	return;
}

int main(void)
{
	ll K; cin >> K;
	using pll = pair<ll,ll>;
	vector<pll> plist;
	prime_factorize(K, plist);

	ll i;
	ll ans = 0;
	for(auto [p,e] : plist)  // p^e
	{
		for(i = p; ; i += p)  // 小さい方からpごと、pで何回割れるか試していく
		{
			// iをpで何回割れるか
			ll j = i;
			while(j%p == 0)
			{
				j /= p;
				e--;
			}
			if(e <= 0)
			{
				chmax(ans, i);
				break;
			}
		}
	}
	cout << ans << endl;

	return 0;
}
