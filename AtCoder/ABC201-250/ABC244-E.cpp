#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;

// ABC244 https://atcoder.jp/contests/abc244

/*
 * 動的計画法で、以下のようにdp[h][j][i]を定める。
 *   A(i)まで動いたときに（＝i回目まで動いたときに）、    i:1～K
 *   頂点jにいて、      j:1～N
 *   かつXに到達した回数が h=0:偶数 h=1:奇数    h:0,1
 * としたときの場合の数
 * 
 * DPの遷移として、
 * 頂点jに繋がっている各頂点について、dp[*][j][i]の値を加える。
 * 次が頂点Xの場合、上記[*]部分を0/1入れ替える。
 * 
 * 最後に(K回動いた後に）、頂点Tにいて、頂点Xに来た回数が偶数回のものを出力すればよい。
 * 
 * graphは[2001][2001]として全探索するとTLEになったので、
 * graph[j]には、頂点jと繋がっているものだけを列挙する。
 */

ll dp[2][2010][2010];
const ll MOD = 998244353;

int main(void)
{
	// 1-indexed
	int h, i, j;
	int N, M, K, S, T, X;
	cin >> N >> M >> K >> S >> T >> X;
	vector<vector<int>> graph(N+1, vector<int>());
	// graph[i]には、頂点iと繋がっている頂点が列挙される
	for(i = 1; i <= M; i++)
	{
		int u, v;
		cin >> u >> v;  // u < v
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	// dp[h][j][i]
	// A(i)まで動いたときに（＝i回目まで動いたときに）、    i:1～K
	// 頂点jにいて、      j:1～N
	// かつXに到達した回数が h=0:偶数 h=1:奇数    h:0,1
	// としたときの場合の数
	dp[0][S][0] = 1;  // 最初は頂点S、かつXに到達した回数は0
	for(i = 0; i <= K; i++)  // 配るDP   0->1回目のぶんからループ
	{
		for(j = 1; j <= N; j++)
		{
			for(auto &e : graph[j])  // j->graph[j][*]マスへの遷移
			{
				if(e != X)  // 次がXマス以外
				{
					dp[0][e][i+1] += dp[0][j][i];
					dp[0][e][i+1] %= MOD;
					dp[1][e][i+1] += dp[1][j][i];
					dp[1][e][i+1] %= MOD;
				}
				else  // 次がXマス  偶奇を入れ替える
				{
					dp[0][e][i+1] += dp[1][j][i];
					dp[0][e][i+1] %= MOD;
					dp[1][e][i+1] += dp[0][j][i];
					dp[1][e][i+1] %= MOD;
				}
			}
		}
	}
	// K回動いた後に、頂点Tにいて、頂点Xに来た回数が偶数回のものを出力
	cout << dp[0][T][K] << endl;

	return 0;
}
