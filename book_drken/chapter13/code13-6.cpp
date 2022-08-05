#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// code13.6の写経

using Graph = vector<vector<int>>;

vector<bool> seen;
vector<int> order;  // トポロジカルソート順

void dfs(Graph &graph, int v)
{
	seen[v] = true;  // 探索済みにする

	for(auto &e : graph[v])
	{
		if(seen[e]) continue;  // 探索済みならスルー
		dfs(graph, e);
	}

	// 再帰関数を抜けるタイミングで、頂点番号を記録する
	order.push_back(v);
}

int main(void)
{
	// 以下、0-indexedかつ有向グラフ
	int i;
	int N, M;  // N:頂点数 M:辺の数
	cin >> N >> M;

	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
	}

	// 探索
	seen.assign(N, false);
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 探索済みならスルー
		dfs(graph, i);
	}

	// 逆順にして出力
	reverse(order.begin(), order.end());
	for(auto &e : order)
	{
		cout << e << ", ";
	}
	cout << endl;

	return 0;
}
