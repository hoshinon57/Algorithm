#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * dp[i][j]を以下のように定義する。
 *   i番目の荷物まで見たときに、  (i:1～N)
 *   重さの総和がj以下である場合の、  (j:0～W)
 *   価値の最大値
 * 
 * dp[i][j] = max(dp[i-1][j], dp[i-1][j-weight]+value)
 * となる。
 * 品物iを選ばない場合と選ぶ場合のうち、最大値の方を適用する考え方。
 */
int main(void)
{
	int i, j;
	ll N, W;
	cin >> N >> W;
	vector<ll> wei(N+1), val(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> wei[i] >> val[i];
	}
	vector<vector<ll>> dp(N+1, vector<ll>(W+1, 0));  // dp[N+1][W+1]

	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= W; j++)
		{
			dp[i][j] = dp[i-1][j];
			if(j-wei[i] >= 0)
			{
				dp[i][j] = max(dp[i][j], dp[i-1][j-wei[i]] + val[i]);
			}
		}
	}
	cout << dp[N][W] << endl;

	return 0;
}
