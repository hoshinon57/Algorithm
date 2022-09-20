#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code16.1の写経
// フォード・ファルカーソン法の実装

// その他参考
//   https://www.slideshare.net/hcpc_hokudai/max-flow
//   競プロにおけるライブラリ化の話もある

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

int main(void)
{
	int N, M;
	cin >> N >> M;
	Graph G(N);
	for(int i = 0; i < M; i++)
	{
		int u, v, c;
		cin >> u >> v >> c;

		// 容量cの辺(u,v)を張る
		G.add_edge(u, v, c);
	}

	FordFulkerson ff;
	int s = 0;
	int t = N-1;
	cout << ff.solve(G, s, t) << endl;

	return 0;
}
