#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC190 https://atcoder.jp/contests/abc190

/*
 * bitDPおよびBFSで解く。メインはbitDP.
 * 解説と同じ考え方。
 *   https://twitter.com/kyopro_friends/status/1355512758482071555
 *   https://atcoder.jp/contests/abc190/editorial/630
 * 
 * 魔法石Ai,Biに辺を張ったグラフを考えると、
 * C1,C2,...,CKを全て通るような最短経路を求める問題になる。
 * Kの最大が17より、bitDPにて求められる。
 * 
 * 各Ci間の距離については、各Ciを起点にBFSを行うことで求められる(K回BFSする)。
 * このとき、Ci間で辿れない組み合わせが1つでもあれば、
 * 最終的に魔法石の列を作ることができないため、-1を出力して終了させてしまってよい。
 * 
 * 最後にbitDPについて、
 * 本問題では一般的な巡回セールスマン問題と違ってスタートとゴールは自由となっている。
 * ゴールについては、bitDP()の引数vを変えながら全K回呼び出すことで対応する。
 * スタートについては、bitDP()におけるベースケースを修正して
 * 「1が立っている個数が1」をスタート地点とすることで対応する。
 */

using Graph_BFS = vector<vector<int>>;
// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph_BFS &G, int s, vector<int> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}
	return;
}

const int MAX_N = 18;  // 頂点数の最大
int dist[MAX_N][MAX_N];  // dist[i][j]:頂点i->jへの距離
// dp[S][v]:
//   Sの各ビットをそれぞれの都市に割り当てて、
//   1になっているビットに対して好きな順に辿ったときの、最小距離
//   ただし最後にvに行く、という制約
//   未探索であれば-1
int dp[1<<MAX_N][MAX_N];

// Sの各ビットをそれぞれの都市に割り当てて、
// 1になっているビットに対して好きな順に辿ったときの、最小距離を返す
// ただし最後にvに行く、という制約 (スタート地点の制約は無い)
// N:頂点数
// 呼び出し元での事前準備：
//   dist[][]に頂点間の距離を設定する
//   dp[][]を未探索の-1にする
int bitDP(int S, int v, int N)
{
	// メモ化再帰で解く
	if(dp[S][v] != -1) return dp[S][v];  // メモ化済み

#if 0
	if(S == 0)  // ベースケース
	{
		// 頂点0をスタートとするので、dp[0][0]は0, dp[0][*]はありえないためINF.
		if(v == 0) dp[S][v] = 0;
		else dp[S][v] = INF32;

		return dp[S][v];
	}
#else
	if(__builtin_popcount(S) == 1)  // ベースケース
	{
		// Sのbitが立っている箇所とbが一致しているか
		if((S & (1<<v)) != 0) dp[S][v] = 0;
		else dp[S][v] = INF32;
		return dp[S][v];
	}
#endif

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
	int i, j;
	int N, M; cin >> N >> M;
	Graph_BFS g_bfs(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g_bfs[a].push_back(b);
		g_bfs[b].push_back(a);
	}

	int K; cin >> K;
	vector<int> c(K); for(i = 0; i < K; i++) {cin >> c[i]; c[i]--;}
	// 各Ci間の距離を、K回BFSすることで求める
	for(i = 0; i < K; i++)  // c[i]からBFS
	{
		vector<int> dist_bfs(N, INF32);
		bfs_graph(g_bfs, c[i], dist_bfs);
		for(j = 0; j < K; j++)
		{
			// Ciのうち辿れない組み合わせが1つでもあれば、列を作ることができない
			if(dist_bfs[c[j]] == INF32)
			{
				cout << -1 << endl;
				return 0;
			}
			dist[i][j] = dist_bfs[c[j]];
		}
	}

	// DPテーブル初期化
	for(i = 0; i < 1<<K; i++)
	{
		for(j = 0; j < K; j++)
		{
			dp[i][j] = -1;  // 未探索
		}
	}

	int ans = INF32;
	for(i = 0; i < K; i++)  // bitDPのゴールを変えながらK回実施
	{
		ans = min(ans, bitDP((1<<K)-1, i, K));
	}
	cout << ans+1 << endl;

	return 0;
}
