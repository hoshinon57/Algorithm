#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 深さ優先探索を用いたトポロジカルソートで解く。
 * 
 * 「有向閉路を含まない」という条件から、
 * トポロジカルソートを使って末尾から距離を確定させていく。
 * 
 * dfs(v)を、頂点vからの最長パスを返すよう定義する。
 * v1からの辺がv1->v2のみであれば、dfs(v1) = dfs(v2)+1 となる。
 * v1からの辺が複数本あれば、dfs(v1) = (dfs(v2)～dfs(vN)の最大値)+1 となる。
 */
using Graph = vector<vector<int>>;

vector<bool> seen;  // 探索済みならtrue メモ化再帰用に使う
vector<int> dist;  // dist[i]:頂点iまでの最長パス

// 頂点vからの最長パスを返す
// トポロジカルソートの末尾から確定させていく形になる
int dfs(Graph &graph, int v)
{
	// メモ化再帰として、すでに探索済みならそれを返す
	if(seen[v]) return dist[v];
	seen[v] = true;

	// dist[]は0で初期化済みなので、末尾の頂点(＝出次数が0の頂点)であれば戻り値は0になる

	for(auto &e : graph[v])
	{
		dist[v] = max(dist[v], dfs(graph, e)+1);  // v->eへ距離が1増えるため、+1する
	}

	return dist[v];
}

int main(void)
{
	// 0-indexedかつ重み無し有向グラフ
	int i;
	int N, M;
	cin >> N >> M;
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int x, y;
		cin >> x >> y;
		x--;  // 0-indexedに変換
		y--;
		graph[x].push_back(y);
	}

	seen.assign(N, false);
	dist.assign(N, 0);
	int answer = -1;
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;
		answer = max(answer, dfs(graph, i));
	}
	cout << answer << endl;
	
	return 0;
}
