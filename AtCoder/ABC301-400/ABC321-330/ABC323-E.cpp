#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC323 https://atcoder.jp/contests/abc323

/*
 * 確率DPで解く。
 * dp[i]を、時刻iにて曲が始まる確率と定義する。
 * 初期値はdp[0]=1.
 * 
 * DP遷移は各時刻i, 曲jについて
 *   dp[i+t[j]] += dp[i]/N
 * となる。
 * 時刻 X-t[0]+1 ～ X の間で曲t[0]が始まれば条件を満たすので、
 * dp[X-t[0]+1]～dp[X] の和をNで割れば答になる。
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

int main(void)
{
	ll i, j;
	ll N, X; cin >> N >> X;
	vector<ll> t(N); for(i = 0; i < N; i++) {cin >> t[i];}
	vector<ll> dp(X+1);
	dp[0] = 1;
	ll N_inv = modinv(N, MOD);

	// 配るDP
	for(i = 0; i <= X; i++)  // 時刻i
	{
		for(j = 0; j < N; j++)  // 曲j
		{
			if(i+t[j]>X) continue;
			dp[i+t[j]] += dp[i]*N_inv;
			dp[i+t[j]] %= MOD;
		}
	}
	ll ans = 0;
	for(i = X-t[0]+1; i <= X; i++)
	{
		if(i < 0) continue;
		ans += dp[i];
		ans %= MOD;
	}
	ans = ans * N_inv % MOD;
	cout << ans << endl;

	return 0;
}
