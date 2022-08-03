#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// code13.4の写経

using Graph = vector<vector<int>>;

vector<bool> seen;

// 再帰を用いた深さ優先探索
// 頂点vからたどることができる頂点のうち、まだ未探索のものを探索していく
void dfs(Graph &graph, int v)
{
	seen[v] = true;  // 探索済みにする

	// 頂点vから行ける各頂点eについて、再帰的に探索
	for(auto &e : graph[v])
	{
		if(seen[e]) continue;  // 探索済みならスルー
		dfs(graph, e);
	}
}

int main(void)
{
	// 以下、0-indexedかつ有向グラフ
	int i;
	int N, M;  // N:頂点数 M:辺の数
	int s, t;  // s:始点 t:終点
	cin >> N >> M >> s >> t;

	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
	}

	// 探索
	seen.assign(N, false);
	dfs(graph, s);  // 始点s

	if(seen[t])  // 終点tにたどり着いたか
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
