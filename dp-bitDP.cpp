#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// bitDPのメモ、ライブラリ

/*
 * ★順列を全探索すれば解ける問題にて、計算量を削減したいときにbitDPが使える。
 *   https://atcoder.jp/contests/abc319/editorial/7117
 * 
 * [参考記事]
 * https://algo-logic.info/bit-dp/
 * 
 * [巡回セールスマン問題について]
 * dp[S][v]を以下のように定める。
 *   Sの各ビットをそれぞれの都市に割り当てて、
 *   1になっているビットに対して好きな順に辿ったときの、最小距離
 *   ただしスタート地点を0とし、最後にvに行く、という制約
 *   未探索であれば-1
 * 
 * 例えばS=b'1101, v=2であれば、
 *   頂点0,2,3を好きな順に辿ったときの、最小距離
 *   ただし最後に行くのは頂点2
 * となる。
 * 
 * dpの遷移式は以下となる。
 *   dp[S][v] = min(dp[S-{v}][u] + dist[u][v])
 *   ※ただしuは、S-{v}に含まれる頂点iそれぞれについて計算する
 * 頂点vを除いた集合の移動距離(終点u) + u->vの距離 という考え方。
 * 
 * [関連問題 / 巡回セールスマン問題]
 * AOJ DPL_2_A https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_2_A&lang=ja
 * ABC190-E  スタートとゴールの考え方が、一般的な巡回セールスマン問題とは少し異なる
 * ABC274-E
 * 
 * [関連問題 / その他のbitDP]  ★再帰を使わない場合、もらうDPより配るDPの方が解きやすいかも？
 * ABC318-D
 * ABC215-E
 * ABC199-E
 * ABC142-E
 */

const int MAX_N = 15;  // 頂点数の最大
int dist[MAX_N][MAX_N];  // dist[i][j]:頂点i->jへの距離
// dp[S][v]:
//   Sの各ビットをそれぞれの都市に割り当てて、
//   1になっているビットに対して好きな順に辿ったときの、最小距離
//   ただし最後にvに行く、という制約
//   未探索であれば-1
int dp[1<<MAX_N][MAX_N];

// Sの各ビットをそれぞれの都市に割り当てて、
// 1になっているビットに対して好きな順に辿ったときの、最小距離を返す
// ただしスタート地点を0とし、最後にvに行く、という制約
// N:頂点数
// 呼び出し元での事前準備：
//   dist[][]に頂点間の距離を設定する
//   dp[][]を未探索の-1にする
int bitDP(int S, int v, int N)
{
	// メモ化再帰で解く
	if(dp[S][v] != -1) return dp[S][v];  // メモ化済み

	if(S == 0)  // ベースケース
	{
		// 頂点0をスタートとするので、dp[0][0]は0, dp[0][*]はありえないためINF.
		if(v == 0) dp[S][v] = 0;
		else dp[S][v] = INF32;

		return dp[S][v];
	}

	if(((S >> v) & 1) == 0) {dp[S][v] = INF32; return dp[S][v];}  // Sに頂点vが含まれていない
	int S_prev = S & (~(1<<v));  // vに相当するビットを落とす

	int &d = dp[S][v];
	d = INF32;
	for(int u = 0; u < N; u++)
	{
		// S_prev内の頂点uを決めて、u->vへの遷移を考える
		if(dist[u][v] == INF32) continue;  // u->vへの辺が無ければスルー
		
		// dp[S][v] = dp[S-{v}][u] + dist[u][v]
		d = min(d, bitDP(S_prev, u, N) + dist[u][v]);
	}
	return d;
}

int main(void)
{
	// 以下はAOJ DPL_2_Aを解く内容
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_2_A&lang=ja
	int i, j;
	int N, M; cin >> N >> M;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			dist[i][j] = INF32;  // 辺で結ばれていない頂点間は距離をINFにする
		}
	}
	for(i = 0; i < M; i++)
	{
		int s, t, d; cin >> s >> t >> d;
		dist[s][t] = d;
	}
	// dpテーブル初期化
	for(i = 0; i < 1<<N; i++)
	{
		for(j = 0; j < N; j++)
		{
			dp[i][j] = -1;  // 未探索
		}
	}

	int ans = bitDP((1<<N)-1, 0, N);
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
