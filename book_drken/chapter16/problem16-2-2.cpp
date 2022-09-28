#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題16.2
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2872&lang=ja

/*
 * 最初に考えた方針。（ただし実装しきれず、解説を見てワーシャルフロイド法でまずは解いた）
 * 最短路となる辺の列挙をワーシャルフロイド法ではなく、ダイクストラ法で求める。
 * 
 * まずはダイクストラ法で各頂点への最短距離(dist[])を求める。
 * 次に終点から始点方向へ、辺(u,v)をそれぞれ列挙して
 *   入力時に与えられた(u,v)間の距離d
 *   ダイクストラ法で求めたdist[]の差
 * がが等しければ、この辺は最短路に含まれると判断できる。
 * この辺を最小カット用のグラフに追加する。
 * 
 * 最後に、フォード・ファルカーソン法で最小カットを求めれば、それが答となる。
 */

// グラフを表す構造体
struct Graph
{
	// 辺を表す構造体
	// rev:逆辺(to,from)がG[to]の中で何番目の要素か
	// cap:辺(from,to)の容量
	struct Edge
	{
		int rev, from, to, cap;
		Edge(int r, int f, int t, int c) : rev(r), from(f), to(t), cap(c) {}
	};

	vector<vector<Edge>> list;

	// N:頂点数
	Graph(int N = 0) : list(N) {}

	// グラフの頂点数を返す
	size_t size(void)
	{
		return list.size();
	}

	// GraphのインスタンスをGとして、G.list[v]をG[v]と書けるようにしておく
	vector<Edge>& operator [] (int v)
	{
		return list[v];
	}

	// 辺e=(u,v)の逆辺e'=(v,u)を取得する
	Edge& rev_edge(const Edge &e)
	{
		return list[e.to][e.rev];
	}

	// 辺e=(u,v)に流量fのフローを流す
	// eの流量がf減少し、逆辺e'=(v,u)の流量はf増加する
	void run_flow(Edge &e, int f)
	{
		e.cap -= f;
		rev_edge(e).cap += f;
	}

	// 頂点fromからtoへ、容量capの辺を張る
	// このとき逆方向to->fromへも容量0の辺を張っておく
	void add_edge(int from, int to, int cap)
	{
		int from_size = (int)list[from].size();
		int to_size = (int)list[to].size();
		list[from].push_back(Edge(to_size, from, to, cap));
		list[to].push_back(Edge(from_size, to, from, 0));
	}
};

struct FordFulkerson
{
	vector<bool> seen;

	FordFulkerson() {}

	// 深さ優先探索を用いて、残余グラフ上でs-tパスを見つける
	// 戻り値：s-tパス上の容量の最小値 パスが見つからなかった場合は0
	// v:探索中の頂点, t:t, f:探索過程の各辺の容量の最小値
	int ff_dfs(Graph &G, int v, int t, int f)
	{
		// 終端に到達したら戻る
		if(v == t) return f;

		seen[v] = true;
		for(auto &e : G[v])  // 頂点vから出ている辺について探索
		{
			if(seen[e.to]) continue;  // 探索済みならスルー
			if(e.cap == 0) continue;  // 容量が0、つまり辺として存在しない

			// vをe.toに更新して、再帰
			// fはe.capとの小さい方を採用する
			// s-tパスが見つからなかった場合、戻り値は0
			int flow = ff_dfs(G, e.to, t, min(f, e.cap));

			if(flow == 0) continue;  // e.to経由ではs-tパスは見つからなかった

			// 辺eに容量flowのフローを流す
			G.run_flow(e, flow);

			// s-tパスを見つけた場合、パス上の最小容量を返す
			return flow;
		}

		return 0;  // s-tパスが見つからなかった
	}

	// グラフGのs-t間の最大流量を求める
	// ただし関数内でGは残余グラフに変化する
	int solve(Graph &G, int s, int t)
	{
		int res = 0;
		
		// 残余グラフにs-tパスが無くなるまで繰り返す
		while(true)
		{
			seen.assign((int)G.size(), false);

			int f = ff_dfs(G, s, t, INF32);
			if(f == 0) break;  // s-tパスが見つからなかったら終了
			res += f;
		}

		return res;
	}
};

// ダイクストラ法 頂点を結ぶ辺
struct D_Edge
{
	int to; // 隣接頂点番号
	int weight;  // 辺の重み
	D_Edge(int to_, int weight_) : to(to_), weight(weight_) {}
};

// ダイクストラ法 重み付きグラフ
// 各頂点が持つのは、頂点ではなく辺
using D_Graph = vector<vector<D_Edge>>;

// ヒープを用いたダイクストラ法
// s,t：始点と終点
void Dijkstra(D_Graph &d_graph, int s, int t, vector<int> &dist)
{
	dist[s] = 0;  // 始点は0に

	// {dist[], v}のペアを要素とした優先度付きキュー
	// dist[]が小さい方が優先度が高い
	priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> que;
	que.push({dist[s], s});

	while(!que.empty())
	{
		// d_min:始点からv_minへの距離
		// v_min:「使用済み」ではない頂点のうち、始点からの距離が最小の頂点
		int v_min = que.top().second;
		int d_min = que.top().first;
		que.pop();

		if(d_min > dist[v_min]) continue;  // {d_min, v_min}はゴミであり、スルー

		// 頂点v_minから出ている各辺について探索
		for(auto &e : d_graph[v_min])
		{
			if(dist[e.to] > dist[v_min] + e.weight)
			{
				dist[e.to] = dist[v_min] + e.weight;
				que.push({dist[e.to], e.to});  // ヒープに挿入
			}
		}
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N, M, s, t;
	cin >> N >> M >> s >> t;
	s--;  // 0-indexedに変換
	t--;
	D_Graph d_graph(N);  // ダイクストラ法で用いるグラフ
	vector<vector<int>> edge_dist(N, vector<int>(N, 0));  // edge_dist[u][v]:u->vを直接つなぐ辺があれば、その距離d
	vector<vector<int>> edge_cost(N, vector<int>(N, 0));  // edge_dist[u][v]:u->vを直接つなぐ辺があれば、そのコストc
	vector<vector<int>> prev_v_list(N);  // prev_v_list[i]:頂点iへと向かう辺の、元の頂点を列挙
	for(i = 0; i < M; i++)
	{
		int u, v, d, c;
		cin >> u >> v >> d >> c;
		u--;  // 0-indexedに変換
		v--;
		d_graph[u].push_back(D_Edge(v, d));  // u->vへ距離d
		edge_dist[u][v] = d;
		edge_cost[u][v] = c;
		prev_v_list[v].push_back(u);  // 頂点vへは、頂点uから直接繋がっている
	}
	// ここまで入力

	// まずはダイクストラ法で各頂点への最短距離を求める
	vector<int> dist(N, INF32);  // 始点から各頂点への距離
	Dijkstra(d_graph, s, t, dist);

	Graph G(N);  // 最小カット用のグラフ
	vector<bool> seen(N, false);
	queue<int> que;
	que.push(t);  // ゴールからスタート方向へ進んでいく
	while(!que.empty())
	{
		int v = que.front();
		que.pop();

		if(seen[v]) continue;  // vについて、vに入る辺を探索完了していれば、スルー

		for(auto &e : prev_v_list[v])
		{
			// 辺(e,v)について、
			//   入力時に与えられた(e,v)間の距離d
			//   ダイクストラ法で求めたdist[]の差
			// が等しければ、この辺は最短路に含まれると判断できる
			if(edge_dist[e][v] == dist[v] - dist[e])
			{
				G.add_edge(e, v, edge_cost[e][v]);  // 最小カット用の辺に追加する
				que.push(e);  // 頂点eに入る辺についても探索する
			}
		}
		seen[v]= true;  // vに入る辺については探索完了
	}

	// フォード・ファルカーソン法で最小カットを求める
	FordFulkerson ff;
	cout << ff.solve(G, s, t) << endl;

	return 0;
}
