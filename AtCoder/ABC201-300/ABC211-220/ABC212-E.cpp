#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC212 https://atcoder.jp/contests/abc212

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i日目が終わったときに、  (i=1～K)
 *   都市jにいるときの、  (j=0～N-1)
 *   旅のしかたの総数
 * dp[K][0]が答となる。
 * 
 * ただし単純に遷移を考えると、(ほぼ)各都市間に道があるため
 * dp[i][*]ごとにO(N^2)の計算量となる。
 * 全体ではO(K*N^2)となり、間に合わない。
 * 
 * 逆に考えて、老朽化により道が無い都市間に辺を張ることを考える。
 * するともらうDPにて
 *   dp[i][j] = (dp[i-1][*]の総和) - (都市jとの道が無い都市xについて、dp[i-1][x]の総和) - dp[i-1][j]
 * となる。
 * 最後のdp[i-1][j]は、同じ都市を結ぶ道は無いため。
 * 制約より、この場合の辺は最大で5000のため、間に合う。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i, j;
	int N, M, K; cin >> N >> M >> K;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	vector<vector<ll>> dp(K+1, vector<ll>(N, 0));  // dp[K+1][N]
	dp[0][0] = 1;  // 初日は都市1からスタート
	ll dp_sum = 1;  // dp[i][*]の総和

	// もらうDP
	for(i = 1; i <= K; i++)
	{
		for(j = 0; j < N; j++)
		{
			ll sub = dp[i-1][j];  // 辺が無いため引く分 都市j->jへの道は無いため引く
			for(auto &e : g[j])  // 都市e->jへの道は無い
			{
				sub += dp[i-1][e];
				sub %= MOD;
			}
			dp[i][j] = (dp_sum + MOD - sub) % MOD;
		}

		dp_sum = 0;
		for(j = 0; j < N; j++)
		{
			dp_sum += dp[i][j];
		}
		dp_sum %= MOD;
	}
	cout << dp[K][0] << endl;

	return 0;
}
