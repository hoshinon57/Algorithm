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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC052 https://atcoder.jp/contests/abc052

/*
 * solve1():
 * Nまでの値をそれぞれ素因数分解し、出てきた素因数ごとにカウントしていくと、
 * N!を p1^e1*p2^e2*... の形で表せる。
 * 約数の個数は　(e1+1)*(e2+1)*... となる。
 * 
 * solve2():
 * Nまでの素数pを列挙し、ルジャンドルの定理で N!がpで何回割れるか を計算する。
 * (各回数+1)の積が答。
**/

// Nを素因数分解し、{素因数, 指数}のpairの形でprime_listに入れる
// 計算量はO(√N)
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

void solve1(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> cnt(N+1);  // cnt[i]: iが約数に出てきた数
	for(i = 2; i <= N; i++)
	{
		vector<pair<ll,ll>> pl;
		prime_factorize(i, pl);
		for(auto [p,e] : pl)
		{
			cnt[p] += e;
		}
	}
	mint ans = 1;
	for(auto &e : cnt)
	{
		ans *= (e+1);
	}
	cout << ans.val() << endl;
}

// 素数列挙(エラトステネスの篩)
// N以下の素数を列挙してprime_listに入れる
// 計算量はO(NloglogN)とのこと。以下2つの記事を参考。
//   https://algo-method.com/descriptions/64
//   https://manabitimes.jp/math/992
void make_prime_list(long long N, vector<long long> &prime_list)
{
	vector<bool> isPrime(N+1, true);
	isPrime[0] = isPrime[1] = false;
	for(long long i = 2; i <= N; i++)
	{
		if(!isPrime[i]) continue;
		// iは素数
		prime_list.push_back(i);
		// iの倍数を素数から外す
		for(long long j = i+i; j <= N; j += i)
		{
			isPrime[j] = false;
		}
	}
	return;
}

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

void solve2(void)
{
	ll N; cin >> N;
	vector<ll> pl;
	make_prime_list(N, pl);
	mint ans = 1;
	for(auto &p : pl)  // 素数p
	{
		ans *= (legendre(N, p) + 1);
	}
	cout << ans.val() << endl;
}

int main(void)
{
//	solve1();
	solve2();

	return 0;
}
