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

// ABC350 https://atcoder.jp/contests/abc350

/*
 * 期待値DPで解く。実装ではメモ化再帰を用いた。
 * 
 * f(n)を以下のように定義する。
 *   整数がnのとき、そこから0にするまでの金額の期待値の最小値
 * f(N)が答。
 * またベースケースとしてf(0)=0.
 * 
 * Xを使うケースを考えると、nからの遷移先はn/Aのため、
 *   f(n) = f(n/A) + X
 * となる。
 * 一方でYを使うケースを考えると、n/1～n/6が同確率であり、
 *   f(n) = (f(n/1) + f(n/2) + ... + f(n/6)) / 6 + Y
 * となる。
 * f(n)が両辺に登場するため（自己ループ）、式変形すると
 *   f(n) = ( (f(n/2) + ... + f(n/6)) / 6 + Y ) * (6/5)
 * となる。
 * 6つの遷移先のうち1つは自己ループのため、
 * 自分以外の遷移先について考えて、6/(6-1)倍すればつじつまが合う、というイメージ。
 * 
 * XとYの両ケースについて、小さい方を採用すればよい。
 */

map<ll,double> memo;  // memo[i]=期待値
ll A, X, Y;

double func(ll n)
{
	if(memo.count(n) > 0)
	{
		return memo[n];
	}
	if(n == 0) return 0.0;

	int i;
	double xx = 0, yy = 0;
	double tmp = 0.0;
	tmp = func(n/A);
	xx = tmp + X;

	tmp = 0.0;
	for(i = 2; i <= 6; i++)
	{
		tmp += func(n/i);
	}
	tmp /= 6.0;
	tmp += Y;
	yy = tmp * 6 / 5;

	memo[n] = min(xx, yy);
	return memo[n];
}

int main(void)
{
	ll N;
	cin >> N >> A >> X >> Y;
	cout << std::fixed << std::setprecision(20);
	cout << func(N) << endl;

	return 0;
}
