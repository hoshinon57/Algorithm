#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題16.3
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2594
// https://atcoder.jp/contests/jag2014autumn/tasks/icpc2014autumn_f

/*
 * 自力で解けず、解説を見て実装した。
 * https://github.com/drken1215/book_algorithm_solution/blob/master/solutions/chap16.md
 * よって解説とほぼ同じコード。
 * （残余グラフにてS,Tに属する頂点を列挙する処理を、DFSでも実装している）
 */

// グラフを表す構造体
struct Graph
{
	// 辺を表す構造体
	// rev:逆辺(to,from)がG[to]の中で何番目の要素か
	// cap:辺(from,to)の容量 最大流を流すことで変化する
	// org_cap:capの初期値
	struct Edge
	{
		int rev, from, to, cap, org_cap;
		Edge(int r, int f, int t, int c) : rev(r), from(f), to(t), cap(c), org_cap(c) {}
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

void dfs_S(int v, vector<int> &zan_S, Graph &g)
{
	zan_S[v] = true;
	for(auto &edge : g[v])
	{
		if(edge.cap > 0 && !zan_S[edge.to])
		{
			dfs_S(edge.to, zan_S, g);
		}
	}
}

void dfs_T(int v, vector<int> &zan_T, Graph &g)
{
	zan_T[v] = true;
	for(auto &edge : g[v])
	{
		if(g.rev_edge(edge).cap > 0 && !zan_T[edge.to])
		{
			dfs_T(edge.to, zan_T, g);
		}
	}
}

bool solve(void)
{
	// 0-indexed
	int i;
	int N, M;
	int s, t;
	cin >> N >> M >> s >> t;
	if(N == 0 && M == 0 && s == 0 && t == 0) return true;  // データ終了
	s--;  // 0-indexedに変換
	t--;
	Graph g(N);
	vector<int> a(M), b(M);
	for(i = 0; i < M; i++)
	{
		cin >> a[i] >> b[i];
		a[i]--;
		b[i]--;
		g.add_edge(a[i], b[i], 1);
	}

	FordFulkerson ff;
	int max_road = ff.solve(g, s, t);

	// 残余グラフにおいて、始点Sからたどれる頂点を調べる
	queue<int> que;
	vector<int> zan_S(N, false);
#if 0  // BFS
	zan_S[s] = true;
	que.push(s);
	while(!que.empty())
	{
		int v = que.front();
		que.pop();

		for(auto &edge : g[v])
		{
			// 辺の容量が残っていて、かつ行き先が未探索の場合
			if(edge.cap > 0 && !zan_S[edge.to])
			{
				zan_S[edge.to] = true;
				que.push(edge.to);
			}
		}
	}
#else  // DFS
	dfs_S(s, zan_S, g);
#endif

	// 残余グラフにおいて、終点Tへとたどれる頂点を調べる
	vector<int> zan_T(N, false);
#if 0 // BFS
	zan_T[t] = true;
	que.push(t);
	while(!que.empty())
	{
		int v = que.front();
		que.pop();

		for(auto &edge : g[v])
		{
			// vへと向かう辺を見たいので、rev_edge()を使う
			// 辺の容量が残っていて、かつ行き先が未探索の場合
			if(g.rev_edge(edge).cap > 0 && !zan_T[edge.to])
			{
				zan_T[edge.to] = true;
				que.push(edge.to);
			}
		}
	}
#else  // DFS
	dfs_T(t, zan_T, g);
#endif

	int answer = 0;
	for(i = 0; i < N; i++)  // 各頂点
	{
		if(zan_T[i])
		{
			for(auto &e : g[i])  // 各辺
			{
				// 始点側への辺であり、辺の容量が残っていて、かつ開始時から変わっていないなら、
				// 反転することで最大流量が増える辺
				if(zan_S[e.to] && e.cap > 0 && e.cap == e.org_cap)
				{
					answer++;
				}
			}
		}
	}

	if(answer > 0)
	{
		cout << max_road+1 << " " << answer << endl;
	}
	else
	{
		cout << max_road << " " << answer << endl;
	}

	return false;  // 次データがある
}

int main(void)
{
	while(true)
	{
		if(solve()) break;
	}
	return 0;
}
