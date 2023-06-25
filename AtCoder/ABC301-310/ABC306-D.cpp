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

// ABC306 https://atcoder.jp/contests/abc306

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i品目までの料理が終わった時に、  (i:1～N)
 *   j=0:お腹を壊していない, j=1:お腹を壊しているときの、
 *   美味しさの総和
 * 
 * DPの遷移は以下のようになる。
 * (1)解毒剤入りの場合
 *   dp[i][0] = max{ dp[i-1][0]から食べる, dp[i-1][1]から食べる, dp[i-1][0]から食べない }
 *   dp[i][1] = dp[i-1][1]から食べない
 * (2)毒入りの場合
 *   dp[i][0] = dp[i-1][0]から食べない
 *   dp[i][1] = max{ dp[i-1][0]から食べる, dp[i-1][1]から食べない }   ※dp[i-1][1]から食べたら死ぬのでNG
 * 
 * dp[N][0], dp[N][1]のうち大きい方が答。
 */
int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<ll> x(N+1), y(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> x[i] >> y[i];
	}
	vector<vector<ll>> dp(N+1, vector<ll>(2, -INF64));  // dp[N+1][2]
	dp[0][0] = 0;

	for(i = 1 ; i <= N; i++)
	{
		ll d1, d2, d3;
		if(x[i] == 0)  // 解毒剤入り
		{
			d1 = dp[i-1][0] + y[i];
			d2 = dp[i-1][1] + y[i];
			d3 = dp[i-1][0];
			dp[i][0] = max({d1, d2, d3});

			dp[i][1] = dp[i-1][1];
		}
		else  // 毒入り
		{
			dp[i][0] = dp[i-1][0];

			// dp[i][1]
			d1 = dp[i-1][0] + y[i];
			d2 = dp[i-1][1];
			dp[i][1] = max({d1, d2});
		}
	}

	cout << max(dp[N][0], dp[N][1]) << endl;

	return 0;
}
