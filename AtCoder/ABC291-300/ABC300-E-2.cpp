#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC300 https://atcoder.jp/contests/abc300

/*
 * メモ化再帰で解く。
 * 再帰関数func(x)を、
 *   数xから操作を始めたときに、最終的に数Nになる確率を(メモ化再帰を用いて)返す
 * と定義する。
 * 
 * x>Nなら0, x=Nなら1を返す。
 * xがメモ未実施の場合、
 *   func(2*x)/5 + func(3*x)/5 + ... + func(6*x)/5
 * が求める値となる。
 * 
 * func(1)が答。
 */

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

const ll div_5 = modinv(5, MOD);  // 5で割る
ll N;
map<ll, ll> dp;
set<ll> dp_memo;

// 数xから操作を始めたときに、最終的に数Nになる確率をメモ化再帰で返す
ll func(ll x)
{
	if(x > N) return 0;
	if(x == N) return 1;

	// メモ化済みならそれを返す
	if(dp_memo.count(x) > 0) return dp[x];

	for(ll i = 2; i <= 6; i++)
	{
		dp[x] += func(x*i)*div_5;
		dp[x] %= MOD;
	}
	dp_memo.insert(x);

	return dp[x];
}

int main(void)
{
	cin >> N;
	cout << func(1) << endl;
	
	return 0;
}
