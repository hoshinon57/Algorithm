#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC010 https://atcoder.jp/contests/abc010

/*
 * 最小カットを求める。
 * 
 * 1つ特別な頂点(終点)を追加し、各女性から終点に辺を張る。
 * すると始点から終点への最小カット問題に置き換えることができる。
 * これをフォード・ファルカーソン法などで解けばよい。
 * 
 * 入力において、友人関係は無向グラフであるため
 * 両方向に辺を張る必要がある点に注意。
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

int main(void)
{
	int i;
	int N, girl, E;
	cin >> N >> girl >> E;
	Graph G(N+1);  // 終点を1つ追加するため、N+1
	for(i = 0; i < girl; i++)
	{
		int v;
		cin >> v;
		G.add_edge(v, N, 1);  // 女性から終点へ辺を張る
	}
	for(i = 0; i < E; i++)
	{
		int u, v;
		cin >> u >> v;
		G.add_edge(u, v, 1);
		G.add_edge(v, u, 1);  // 無向グラフなので、反対方向にも辺を張る
	}
	// ここまで入力

	// フォード・ファルカーソン法で最小カットを求める
	FordFulkerson ff;
	int s = 0;  // 始点
	int t = N;  // 追加した終点
	cout << ff.solve(G, s, t) << endl;

	return 0;
}
