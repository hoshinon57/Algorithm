#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// 章末問題13.1

/*
 * 深さ優先探索、幅優先探索それぞれで解く。
 * 
 * 深さ優先探索：
 * 頂点vを起点に、graphからたどれる頂点を全て探索し、seen[]を更新するdfs()を用意する。
 * dfs()呼び出しは頂点vが未探索のものに対して行う。
 * するとdfs()呼び出しの回数が連結成分の個数になる。
 * 
 * 幅優先探索：
 * dfsと同様。
 * 頂点sを起点に、graphからたどれる頂点を全て探索し、seen[]を更新するbfs()を用意する。
 * bfs()呼び出しは頂点vが未探索のものに対して行う。
 * するとbfs()呼び出しの回数が連結成分の個数になる。
 */

using Graph = vector<vector<int>>;

vector<bool> seen;  // 探索済みならtrue

// 再帰を用いた深さ優先探索
// 頂点vを起点に、graphからたどれる頂点を全て探索し、seen[]を更新する
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

// 幅優先探索
// 頂点sを起点に、graphからたどれる頂点を全て探索し、seen[]を更新する
void bfs(Graph &graph, int s)
{
	queue<int> que;

	// 頂点sを始点とする
	seen[s] = true;
	que.push(s);

	while(!que.empty())
	{
		// キューから頂点を1つ取得し、それをキューから削除する
		int v = que.front();
		que.pop();
		for(auto &e : graph[v])  // 頂点vと接続している頂点を、1つずつeに設定
		{
			if(seen[e]) continue;  // 探索済みならスルー

			// 探索済みにしてキューに積む
			seen[e] = true;
			que.push(e);
		}
	}
}

int main(void)
{
	// 以下、0-indexedかつ無向グラフ
	int i;
	int answer;
	int N, M;  // 頂点数と辺数
	cin >> N >> M;

	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}

	// 深さ優先探索で解くパターン
	seen.assign(N, false);
	answer = 0;
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 探索済みならスルー
		dfs(graph, i);
		answer++;
	}
	cout << "dfs:" << answer << endl;

	// 幅優先探索で解くパターン
	seen.assign(N, false);
	answer = 0;
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 探索済みならスルー
		bfs(graph, i);
		answer++;
	}
	cout << "bfs:" << answer << endl;

	return 0;
}
