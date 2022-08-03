#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// code13.5の写経

using Graph = vector<vector<int>>;

vector<int> color;  // 頂点の色 -1:未探索 0:白 1:黒

// 再帰を用いた深さ優先探索
// 頂点vを色cに塗り、vと接続する頂点を別の色で塗る
// 別の色で塗れればtrue、矛盾が発生すればfalseを返す
bool dfs(Graph &graph, int v, int c)
{
	color[v] = c;  // 色を塗る

	// 頂点vから行ける各頂点eについて、再帰的に探索
	for(auto &e : graph[v])
	{
		if(color[e] == 1-c) continue;  // 別の色で塗り済みならOK スルー
		if(color[e] == c) return false;  // 隣の頂点と同じ色のためNG

		// 未探索の頂点
		// 探索先からfalseで返ってきたら、ここでもfalseを返す
		if(!dfs(graph, e, 1-c)) return false;
	}

	return true;
}

int main(void)
{
	// 以下、0-indexedかつ無向グラフ
	int i;
	int N, M;  // N:頂点数 M:辺の数
	cin >> N >> M;

	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}

	// 探索
	bool answer = true;
	color.assign(N, -1);  // 全ての頂点を未探索に
	for(i = 0; i < N; i++)
	{
		if(color[i] != -1) continue;  // 探索済みならスルー
		// まずは白に塗って探索開始
		if(!dfs(graph, i, 0))
		{
			answer = false;  // 途中で矛盾が発生した
		}
	}

	if(answer) cout << "Yes" << endl;
	else cout << "No" << endl;

	return 0;
}
