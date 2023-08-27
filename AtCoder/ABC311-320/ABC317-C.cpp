#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC317 https://atcoder.jp/contests/abc317

/*
 * 深さ優先探索で解く。
 * 
 * dfs():引数で与えられた頂点vを起点にDFSする を用意する。
 * 辺の長さの和を更新しながら、未探索の頂点へ再帰していく。
 * これをmain()から各頂点ごとに呼び出し、長さの最大値を更新/保持していけばよい。
 * 
 * 計算量は、main()からのdfs()呼び出しが1回あたりO(N!)であるため、
 * 全体でO(N*N!).
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

int ans = 0;

// 頂点vを起点にDFS.
// sum:現時点の辺の長さの和
void dfs(Graph &g, int v, vector<bool> &seen, int sum = 0)
{
	seen[v] = true;
	ans = max(ans, sum);
	for(auto &e : g[v])
	{
		if(seen[e.to]) continue;
		dfs(g, e.to, seen, sum+e.weight);
	}
	seen[v] = false;  // 帰りがけ時に未探索に戻す
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b, c; cin >> a >> b >> c;
		a--; b--;
		g[a].push_back(Edge(b, c));
		g[b].push_back(Edge(a, c));
	}

	vector<bool> seen(N, false);
	for(i = 0; i < N; i++)  // 各頂点を起点にdfs
	{
		dfs(g, i, seen);
	}
	cout << ans << endl;

	return 0;
}
