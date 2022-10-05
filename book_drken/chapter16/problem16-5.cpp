#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題16.5
// https://atcoder.jp/contests/soundhound2018/tasks/soundhound2018_c

/*
 * 自力で解けず、解説を見て実装した。
 * https://drken1215.hatenablog.com/entry/2019/06/17/221400
 * https://blog.hamayanhamayan.com/entry/2018/01/28/172047
 * https://tutuz.hateblo.jp/entry/2018/08/15/213658
 * （ただし解説にてフォード・ファルカーソン法で解いたソースコードは見つからなかった）
 * 
 * 以下、「座標x,yの和が偶数の頂点」を偶数頂点と書く。
 * 同じく奇数頂点と書く。
 * 
 * 二分グラフにおける最大安定集合を求めることと同じ意味になる。
 * これは「頂点数 - 二部マッチングの最大サイズ」で求められる。
 * 
 * 二部マッチングの最大サイズを求めるには、
 * 偶数頂点から奇数頂点へとフローを流すことを考える。
 * (1)偶数頂点をまとめる頂点
 * (2)奇数頂点をまとめる頂点
 * を用意して、(1)->偶数頂点->奇数頂点->(2)の方向へフローを流せばよい。
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
	// 0-indexed
	int i, j;
	int r, c;
	cin >> r >> c;
	vector<vector<char>> a(r);
	int v = 0;  // '.'のマスの数
	// 最大流を流す用のグラフ
	// 「頂点をまとめる頂点」を2つ用意するので、+2
	// 「偶数頂点をまとめる頂点」はr*c, 「奇数頂点をまとめる頂点」はr*c+1 が頂点番号になる
	Graph G(r*c+2);
	for(i = 0; i < r; i++)
	{
		a[i].resize(c);
		for(j = 0; j < c; j++)
		{
			cin >> a[i][j];
			if(a[i][j] == '.') v++;
			if((i+j)%2 == 0)
			{
				G.add_edge(r*c, i*c+j, 1);  // 偶数頂点をまとめる頂点 -> 偶数頂点
			}
			else
			{
				G.add_edge(i*c+j, r*c+1, 1);  // 奇数頂点 -> 奇数頂点をまとめる頂点
			}
		}
	}

	int dx[4] = {1, -1, 0,  0};
	int dy[4] = {0,  0, 1, -1};
	for(i = 0; i < r; i++)
	{
		for(j = 0; j < c; j++)
		{
			// 偶数頂点->奇数頂点の方向へ辺を張る
			if(a[i][j] == '*') continue;  // 壁ならスルー
			if((i+j)%2 == 1) continue;

			for(int d = 0; d < 4; d++)  // 上下左右4方向
			{
				// 上下左右のマスが、範囲内かつ壁ではないこと
				int dj = j+dx[d];
				int di = i+dy[d];
				if(dj < 0 || dj >= c) continue;
				if(di < 0 || di >= r) continue;
				if(a[di][dj] == '*') continue;

				G.add_edge(i*c+j, di*c+dj, 1);
			}
		}
	}

	FordFulkerson ff;
	int s = r*c;
	int t = r*c+1;
	// 「頂点数(この場合は'.'のマスの数) - 二部マッチングの最大サイズ」が求める答になる
	cout << v - ff.solve(G, s, t) << endl;

	return 0;
}
