#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code14.5の写経および改善

/*
 * ワーシャルフロイド法では、負の辺を持つ場合に注意。
 * ・頂点i,jが繋がっていない
 * ・i->kのコストが-5, k->jのコストがINF(繋がっていない)
 * としたときに、
 * DPの更新式
 *   dp[i][j] = min(dp[i][j], dp[i][k]+dp[k][j]);
 * において、dp[i][j]がINFより小さな値に更新されてしまう。
 * 
 * 解決方法としては以下の2点。
 * (1)
 * 出力において、 if(dp[i][j] > INF64/2) のように上限判定を緩くする。
 * 本のコードや以下サイトで採用されている方法。
 *   https://algo-logic.info/warshall-floyd/
 * ※ただしどちらも「なぜ2で割っているか」の説明はされていなかった。
 * 
 * (2)
 * DP更新式において、以下のようにINF判定を追加する。
 * if(dp[i][k] != INF64 && dp[k][j] != INF64)
 *   dp[i][j] = min(dp[i][j], dp[i][k]+dp[k][j]);
 * 参考：
 *   https://kujira16.hateblo.jp/entry/2014/05/14/220520
 *   https://37zigen.com/floyd-warshall-algorithm/
 * 
 * (2)の方が個人的には好みだが、この除外判定で全てのケースを正しく判定できるのかは未確認。
 * おそらく大丈夫と思うが…
 */
int main(void)
{
	// 以下、0-indexedかつ重み付き有向グラフ
	int i, j, k;
	int N, M;  // 頂点の数、辺の数
	cin >> N >> M;

	// dp[k][i][j]
	//   頂点0, 1, ..., k-1のみを中継頂点として通ってよいとした場合の、   k:0～N
	//   頂点iからjへの最短路長   i,j:0～N-1
	//   ただし[k]は使わず、in-placeで実装できる。
	// DP初期条件は、
	//   dp[0][i][j] =
	//     0(i=jのとき),
	//     (頂点i->jへの辺の重み) (i,jが直接つながっているとき),
	//     INF(それ以外)
	// 遷移式は
	//   dp[k+1][i][j] = min(dp[k][i][j], dp[k][i][k]+dp[k][k][j])  // kを経由しない場合とする場合との比較
	vector<vector<ll>> dp(N, vector<ll>(N, INF64));  // 距離は無限大で初期化
	for(i = 0; i < M; i++)
	{
		int a, b;
		ll w;
		cin >> a >> b >> w;
		dp[a][b] = w;  // 頂点a,bが直接つながっているときの、DP初期条件
	}
	for(i = 0; i < N; i++) dp[i][i] = 0;  // 自分自身との距離は0

	// ワーシャルフロイド法を用いて解く
	for(k = 0; k < N; k++)
	{
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				if(dp[i][k] != INF64 && dp[k][j] != INF64) dp[i][j] = min(dp[i][j], dp[i][k]+dp[k][j]);  // 負辺がある場合の解決方法 その1
//				dp[i][j] = min(dp[i][j], dp[i][k]+dp[k][j]);
			}
		}
	}

	// dp[v][v] < 0 が存在する場合、負閉路がある
	bool exist_negative_cycle = false;
	for(i = 0; i < N; i++)
	{
		if(dp[i][i] < 0) exist_negative_cycle = true;
	}

	if(exist_negative_cycle)
	{
		cout << "exist negative cycle." << endl;
	}
	else
	{
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				cout << std::setw(4);
				if(dp[i][j] == INF64)
//				if(dp[i][j] > INF64/2)  // 負辺がある場合の解決方法 その2
				{
					cout << "INF";
				}
				else
				{
					cout << dp[i][j];
				}
			}
			cout << endl;
		}
	}

	return 0;
}
