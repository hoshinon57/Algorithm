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

// ABC220 https://atcoder.jp/contests/abc220

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   操作をi回実施したのち、
 *   左端の値(=挿入した値)がjであるような
 *   操作手順の個数
 * 
 * DP初期値は dp[0][A1] = 1 となる。
 * 遷移は配るDPにて、
 * 前回操作で挿入した値(=dp[i-1][*])とa[i]を用いて、次に挿入する値を決定する。
 * 
 * dp[N-1][*]が答となる。
 */

int main(void)
{
	int i, j;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<vector<ll>> dp(N, vector<ll>(10, 0));  // dp[N][10]
	dp[0][a[0]] = 1;  // DP初期値

	// 配るDP
	for(i = 1; i < N; i++)  // 操作i回目
	{
		for(j = 0; j <= 9; j++)  // 左端の値
		{
			int nxt;
			// 操作F
			nxt = (j+a[i])%10;
			dp[i][nxt] += dp[i-1][j];
			dp[i][nxt] %= MOD;

			// 操作G
			nxt = (j*a[i])%10;
			dp[i][nxt] += dp[i-1][j];
			dp[i][nxt] %= MOD;
		}	
	}

	for(j = 0; j <= 9; j++)
	{
		cout << dp[N-1][j] << endl;
	}

	return 0;
}
