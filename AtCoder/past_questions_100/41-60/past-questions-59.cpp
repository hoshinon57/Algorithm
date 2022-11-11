#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int MOD = 10000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題59
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2014yo/tasks/joi2014yo_e

/*
 * 幅優先探索とダイクストラ法で解く。
 * 
 * まずは各町について、R本以内で行ける町に対してコストCの辺を張る。これは幅優先探索で求める。
 * それを元にダイクストラ法で町1から町Nへの最小コストを求めればよい。
 * 
 * 計算量は、
 *   それぞれの町について幅優先探索をする部分が、O(N(N+K)).
 *   ダイクストラ法の部分が、O(KlogN).
 * よって O(N(N+K) + KlogN) かな…と考える。
 */

// 1つの辺を表す構造体
struct Edge
{
	int to;  // 辺の行き先
	ll w;  // 辺の重み
	Edge(int _to, ll _w) : to(_to), w(_w) {};
};

// それぞれ、幅優先探索用、ダイクストラ用のグラフ
using G_BFS = vector<vector<int>>;
using G_DIJ = vector<vector<Edge>>;
G_BFS g_bfs;
G_DIJ g_dij;

int N, K;

// 始点sから距離r以内の町を幅優先探索で列挙し、
// ダイクストラ用にコストcの辺を張る
void bfs(int s, int c, int r)
{
	queue<int> que;
	vector<ll> dist(N, INF64);  // dist[i]:始点から町iへの距離

	// 初期設定 始点s
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();

		for(auto &e : g_bfs[v])  // e:町vと接する町
		{
			if(dist[e] == INF64)
			{
				dist[e] = dist[v] + 1;

				// 始点->町eへダイクストラ用の辺を張る
				g_dij[s].push_back(Edge(e, c));  // コストはcで固定

				if(dist[e] + 1 <= r)  // 町eの次の町もタクシーが行ける場合、キューに積む
				{
					que.push(e);
				}
			}
		}
	}
}

// s->tへの最小運賃をダイクストラ法で解き、その最小運賃を返す
ll Dijkstra(int s, int t)
{
	using pll = pair<ll, ll>;  // {最小運賃, その町}
	// pair<ll,ll>型の要素を持ち、小さい方が優先度が高いキュー
	priority_queue<pll, vector<pll>, greater<pll>> que;
	vector<ll> cost(N, INF64);  // cost[i]:始点から町iへの最小運賃

	// 初期設定 町sが始点
	cost[s] = 0;
	que.push({cost[s], s});

	while(!que.empty())
	{
		ll c = que.top().first;
		ll v = que.top().second;
		que.pop();

		if(c > cost[v]) continue;  // キューから取り出した値より、さらに小さい値で更新されていたらスルー

		for(auto &e : g_dij[v])  // e:町vからの辺
		{
			if(cost[e.to] > cost[v] + e.w)  // 運賃を更新できる場合
			{
				cost[e.to] = cost[v] + e.w;
				que.push({cost[e.to], e.to});
			}
		}
	}

	return cost[t];
}

int main(void)
{
	// 0-indexed
	int i;
	cin >> N >> K;
	g_bfs.resize(N);
	g_dij.resize(N);
	vector<int> c(N), r(N);
	for(i = 0; i < N; i++)
	{
		cin >> c[i] >> r[i];
	}
	for(i = 0; i < K; i++)
	{
		int a, b;
		cin >> a >> b;
		a--; b--;  // 0-indexedに変換
		g_bfs[a].push_back(b);
		g_bfs[b].push_back(a);
	}

	// 幅優先探索で、各町を起点にタクシーで行ける町を列挙し、ダイクストラ用の辺を張る
	for(i = 0; i < N; i++)
	{
		bfs(i, c[i], r[i]);
	}

	// ダイクストラ法で、町0から町N-1への最小運賃を求める
	cout << Dijkstra(0, N-1) << endl;

	return 0;
}
