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
 * 別解の、二分探索＆ルジャンドルの定理を用いて解く。
 *   https://atcoder.jp/contests/abc280/editorial/5348
 * 
 * x!はKの倍数か？の判定問題を考えると、
 * これはfalse,false,...,false,true,true,... とf/tがある1つの点で切り替わる。
 * よって二分探索が使える。
 * 
 * 判定問題を解くにはルジャンドルの定理を用いる。
 * K=p^e*p^e*p^e*... としたときに、各p,eについて
 *   (x!をpで割り切れる回数) >= e
 * であれば良い。
 */

// n!が素数pで割り切れる回数を返す
ll legendre(ll n, ll p)
{
	ll ret = 0;
	while(n > 0)
	{
		ret += n/p;
		n /= p;
	}
	return ret;
}

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
	vector<pair<ll,ll>> plist;
	prime_factorize(K, plist);

	// x!がKの倍数であればtrue
	auto check = [&](ll x) -> bool
	{
		for(auto [p,e] : plist)
		{
			if(legendre(x, p) >= e) continue;
			else return false;
		}
		return true;
	};
	ll ng = 1;  // K>=2のため、n=1は確実にNG
	ll ok = K;  // K!はKの倍数
	while(ok - ng > 1)
	{
		ll mid = (ng+ok) / 2;
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
