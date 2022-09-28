#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題16.4
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1163&lang=jp

/*
 * 二部マッチング問題をフォード・ファルカーソン法で解く。
 * 
 * 「blueをまとめる頂点」と「redをまとめる頂点」を用意し、
 * 前者からは全てのblue頂点へ、
 * 全てのred頂点からは後者の頂点へ、それぞれ辺を繋げる。
 * 
 * また全てのblue,redの組み合わせにて、最大公約数が2以上であれば
 * それらの頂点をblue->red方向へ辺を繋げる。
 * 
 * 最後に、フォード・ファルカーソン法を用いて
 *   blueをまとめる頂点->redをまとめる頂点
 * への最大流を求めればよい。
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

int gcd(int a, int b)
{
	if(b == 0) return a;
	return gcd(b, a%b);
}

int solve(int m, int n, vector<int> &blue, vector<int> &red)
{
	// 頂点番号 0～m-1:blue, m～m+n-1:red, m+n:blueをまとめる頂点, m+n+1:redをまとめる頂点
	Graph g(n+m+2);

	// 頂点の並べ方は、 blueをまとめる頂点 -> blue -> red -> redをまとめる頂点
	int i, j;
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(gcd(blue[i], red[j]) > 1)  // 互いに素でなければ、最大流のグラフに追加する
			{
				g.add_edge(i, m+j, 1);  // blue->red方向 辺の容量は1
			}
		}
	}

	for(i = 0; i < m; i++)  // 各blue頂点と、blueをまとめる頂点を結ぶ
	{
		g.add_edge(m+n, i, 1);
	}
	for(i = 0; i < n; i++)  // 各red頂点と、redをまとめる頂点を結ぶ
	{
		g.add_edge(m+i, m+n+1, 1);
	}
	
	FordFulkerson ff;
	return ff.solve(g, m+n, m+n+1);
}

int main(void)
{
	// 0-indexed
	while(true)
	{
		int m, n;
		cin >> m >> n;
		if(m == 0 && n == 0) break;

		vector<int> blue(m);
		vector<int> red(n);
		int i;
		for(i = 0; i < m; i++) cin >> blue[i];
		for(i = 0; i < n; i++) cin >> red[i];

		cout << solve(m, n, blue, red) << endl;
	}

	return 0;
}
