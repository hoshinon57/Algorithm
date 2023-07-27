#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC261 https://atcoder.jp/contests/abc261

/*
 * 動的計画法で解く。
 * 
 * ・dp[]の決め方
 * ・dpの遷移
 * など以下コード中のコメントを参照。
 */
int main(void)
{
	// 1-indexed
	ll i, j;
	ll N, M;
	cin >> N >> M;
	vector<ll> x(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> x[i];
	}
	vector<ll> c(M+1);
	vector<ll> y(M+1);
	vector<ll> bonus(N+1);  // bonus[i]:カウンタがiになったときのボーナス(ボーナスが無い場合は0)
	for(i = 1; i <= M; i++)
	{
		cin >> c[i] >> y[i];
		bonus[c[i]] = y[i];
	}

	// dp[i][j]:
	//   i:i回目までコイントスを行った後に、  1～N
	//   j:カウンタがjのときの、  0～N
	//   もらえるお金の最大値
	// コイントス1回実施後にカウンタが10など、遷移としてありえない箇所は-1とする。
	vector<vector<ll>> dp(N+1, vector<ll>(N+1, -1));
	dp[0][0] = 0;  // コイントス開始前は、カウンタ0のみありうる(お金0)。他はありえないので-1.
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= N-1; j++)
		{
			// もらうDP
			// 前回のコイントスでカウンタがjのときからの遷移を考える。
			// 表が出る場合：
			//   カウンタが1増加、x[i]円もらえる、またボーナスももらえるので、
			//   dp[i][j+1] と dp[i-1][j]+x[i]+bonus[j+1]  のうち大きい方をdp[i][j+1]に設定する。
			// 裏が出る場合：
			//   カウンタが0になるので、
			//   dp[i][0] と dp[i-1][j]  のうち大きい方をdp[i][0]に設定する。

			if(dp[i-1][j] == -1) continue;  // 通りえないところからは遷移しない

			// 表
			ll tmp;
			tmp = dp[i-1][j]+x[i]+bonus[j+1];
			dp[i][j+1] = max(dp[i][j+1], tmp);

			// 裏
			dp[i][0] = max(dp[i][0], dp[i-1][j]);
		}
	}

	// カウンタの値に関係なく、コイントスをN回実施した後の最大値を出力すればよい
	ll ans = 0;
	for(i = 0; i <= N; i++)
	{
		ans = max(ans, dp[N][i]);
	}
	cout << ans << endl;

	return 0;
}
