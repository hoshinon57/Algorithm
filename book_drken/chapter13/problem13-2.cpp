#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// 章末問題13.2

/*
 * 頂点sを起点に、graphからたどれる頂点を全て探索し、seen[]を更新するbfs()を用意する。
 * これを入力の頂点sに対して呼び出した後に、
 * 頂点vのseen[]がtrueかfalseかで、s-tパスの存在を判定できる。
 */

using Graph = vector<vector<int>>;

vector<bool> seen;

// 幅優先探索
// 頂点sを起点に、graphからたどれる頂点を全て探索し、seen[]を更新する
void bfs(Graph &graph, int s)
{
	queue<int> que;

	// 頂点sを始点とする
	seen[s] = true;
	que.push(s);

	// 幅優先探索
	while(!que.empty())
	{
		// キューから頂点を1つ取得し、それをキューから削除する
		int v = que.front();
		que.pop();

		for(auto &e : graph[v])  // 頂点vと接続している頂点を、1つずつeに設定
		{
			if(seen[e]) continue;  // 探索済みならスルー

			seen[e] = true;
			que.push(e);
		}
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
	bfs(graph, s);  // 始点s

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
