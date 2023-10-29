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

// ABC326 https://atcoder.jp/contests/abc326

/*
 * 動的計画法で解く。(確率DP)
 * 以下、1-indexedで考える。
 * 
 * dp[i]を以下のように定義する。
 *   現在iマスにいるとき、以降でもらえる期待値
 *   (A[i]の支給はカウント外)
 * 
 * 現在マスiにいるとき、i+1以上のサイコロが出ないと終了なので、
 *   dp[i] = (A[i+1]+dp[i+1])/N + (A[i+2]+dp[i+2])/N + ... + (A[N]+dp[N])/N
 *         = Σ(A[k]+dp[k]) / N   (k:i+1～N)
 * となる。  
 * dp[N]=0で、dp[]を後ろから求めていけばよい。
 * ただし Σ(A[k]+dp[k]) の部分を毎回愚直に求めると、全体でO(N^2)となってしまうため、
 * 累積和で計算する必要がある。
 * 
 * dp[0]が答。
 * 
 * [ACまでの思考の流れ]
 * ・パッと見で確率DP.
 * ・dp[i]を(前述のように)定義したときに、どう遷移できるか。
 * ・ゴール側から計算していった方が良さそう。
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
	// 1-indexed
	ll i;
	ll N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<ll> dp(N+1);  // dp[i]:現在iマスにいるとき、以降でもらえる期待値
	ll sum;  // (a[i]+dp[i])から(a[N]+dp[N])までの和
	dp[N] = 0;
	sum = a[N] + dp[N];

	ll N_inv = modinv(N, MOD);
	for(i = N-1; i >= 0; i--)
	{
		dp[i] = sum * N_inv % MOD;
		sum = (sum + a[i] + dp[i]) % MOD;
	}
	cout << dp[0] << endl;

	return 0;
}
