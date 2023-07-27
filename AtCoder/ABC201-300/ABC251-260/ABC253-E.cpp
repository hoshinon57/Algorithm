#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;
const ll MOD = 998244353;

// ABC253 https://atcoder.jp/contests/abc253

/*
 * (1-indexed)
 * 累積和を用いた動的計画法で解く。
 * 
 * dp[i][j]を、
 *   i:A(i)まで見たときに		i:1～N
 *   j:A(i)=jであるときの		j:1～M
 *   組み合わせの数
 * とする。
 * 
 * もらうDPのとき、遷移は、
 * (1) A(i)<A(i+1)のとき
 *   A(i)は 1～A(i+1)-K の範囲になるので、
 *   dp[i+1][j] = dp[i][1] + dp[i][2] + … + dp[i][j-K]
 * (2) A(i)>=A(i+1)のとき
 *   A(i)は A(i+1)+K～M の範囲になるので、
 *   dp[i+1][j] = dp[i][j+K] + dp[i][j+K+1] + … + dp[i][M]
 * をdp[i][j]の各jごとに計算する。
 * 
 * ただしそのままでは計算量がO(N*M*M)となりTLE.
 * そのため、累積和として
 *   dpsum[i][j]:dp[i][1]～dp[i][j]までの和
 * を定義すればO(N*M)になる。
 * 
 * 注意点として、K=0のときは上記(1),(2)で計算が被ってしまう。
 * K=0のときだけ処理を変える必要がある。
 */
int main(void)
{
	// 1-indexed
	ll h, i, j;
	ll N, M, K;
	cin >> N >> M >> K;

	// dp[i][j]
	// i:A(i)まで見たときに		i:1～N
	// j:A(i)=jであるときの		j:1～M
	// 組み合わせの数
	vector<vector<ll>> dp(N+1, vector<ll>(M+1, 0));
	vector<vector<ll>> dpsum(N+1, vector<ll>(M+1, 0));  // dpsum[i][j]:dp[i][1]～dp[i][j]までの和
	for(j = 1; j <= M; j++)
	{
		dp[1][j] = 1;  // A(1)は全てありえる
		dpsum[1][j] = dpsum[1][j-1]+dp[1][j];
	}

	// もらうDP
	for(i = 2; i <= N; i++)
	{
		for(j = 1; j <= M; j++)
		{
			// A(i) < A(i+1) のケース
			h = j-K;
			if(h >= 1)
			{
				dp[i][j] += dpsum[i-1][h];  // dp[i-1][1]～dp[i-1][h]までの和を足せばよい
				dp[i][j] = dp[i][j] % MOD;
			}
/*
			// 累積和を使わない場合は以下。
			for(h = j-K; h >= 1; h--)
			{
				dp[i][j] += dp[i-1][h];
				dp[i][j] = dp[i][j] % MOD;
			}			
*/
			// A(i) >= A(i+1) のケース
			// K=0のときは計算が異なるため注意 (A(i)<A(i+1)のケースと計算が被ってしまう)
			if(K != 0)
			{
				h = j+K;
				if(h <= M)
				{
					dp[i][j] += (dpsum[i-1][M] + MOD - dpsum[i-1][h-1]);
					dp[i][j] = dp[i][j] % MOD;
				}
			}
			else
			{
				h = j+K;
				if(h <= M)
				{
					dp[i][j] += (dpsum[i-1][M] + MOD - dpsum[i-1][h]);
					dp[i][j] = dp[i][j] % MOD;
				}
			}
/*
			// 累積和を使わない場合は以下。
			for(h = j+K; h <= M; h++)
			{
				dp[i][j] += dp[i-1][h];
				dp[i][j] = dp[i][j] % MOD;
			}
*/
			dpsum[i][j] = dpsum[i][j-1] + dp[i][j];
		}
	}

	ll answer = 0;
	for(j = 1; j <= M; j++)
	{
		answer += dp[N][j];
		answer = answer % MOD;
	}
	cout << answer << endl;

	return 0;
}
