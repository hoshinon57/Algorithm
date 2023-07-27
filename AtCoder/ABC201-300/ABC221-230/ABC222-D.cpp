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

// ABC222 https://atcoder.jp/contests/abc222

/*
 * 動的計画法と累積和で解く。
 * 以下、0-indexedとする。
 * 
 * dp[i][j]を以下のように定める。
 *   A[i],B[i]まで見たときに、  (i:0～N-1)
 *   C[i]=jを選んだときの、    (j:0～3000)
 *   場合の数
 * DP遷移は dp[i][j] = dp[i-1][0]～dp[i-1][j]の和 となる。
 * 答はdp[N-1][*]の和。
 * 
 * ただしDP遷移を単純に計算すると、
 * M=3000として計算量がO(N*M^2)となりTLE.
 * そのため、"dp[i-1][0]～dp[i-1][j]の和" の部分について累積和を用いてO(1)で計算する。
 */

int main(void)
{
	// 0-indexed
	int N;
	cin >> N;
	int i, j;
	vector<ll> a(N), b(N);
	for(i = 0; i < N; i++) cin >> a[i];
	for(i = 0; i < N; i++) cin >> b[i];
	
	const int NUM_MAX = 3000;
	vector<vector<ll>> dp(N, vector<ll>(NUM_MAX+1));
	vector<ll> dp_sum(NUM_MAX+2);  // 累積和
	// i=0
	for(i = a[0]; i <= b[0]; i++)
	{
		dp[0][i] = 1;
	}
	// i=1以降
	// もらうDP
	for(i = 1; i < N; i++)
	{
		// dp[i-1][*]の累積和を計算
		for(j = 0; j <= NUM_MAX; j++)
		{
			dp_sum[j+1] = (dp_sum[j] + dp[i-1][j])%MOD;
		}

		// dp[i][j] = dp[i-1][0]～[j]の和 -> 半開区間で[0,j+1)
		for(j = a[i]; j <= b[i]; j++)
		{
			dp[i][j] = dp_sum[j+1];
		}
	}

	// dp[N-1][*]の和が答となる
	ll answer = 0;
	for(i = 0; i <= NUM_MAX; i++)
	{
		answer += dp[N-1][i];
		answer %= MOD;
	}
	cout << answer << endl;

	return 0;
}
