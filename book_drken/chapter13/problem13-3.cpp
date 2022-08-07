#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// 章末問題13.3

/*
 * 頂点sを起点に、graphからたどれる頂点を全て探索し、color[]を更新する関数bfs()を用意する。
 * まだ色を塗っていない全ての頂点に対してbfs()を呼び出し、
 * 最後まで矛盾が発生しなければ二分グラフと判定する。
 */

using Graph = vector<vector<int>>;

vector<int> color;  // 頂点の色 -1:未探索 0:白 1:黒

// 幅優先探索
// 頂点sを起点に、graphからたどれる頂点を全て探索し、color[]を更新する。
// 始点の色color[s]は0で固定とし、隣り合う頂点は別の色で塗っていく。
// 「塗ろうとした隣の色が、今の色と同じ」という矛盾が発生したら二分グラフではないと判断し、falseを返す。
bool bfs(Graph &graph, int s)
{
	queue<int> que;

	// 頂点sを始点とする
	color[s] = 0;  // 始点は白とする
	que.push(s);

	// 幅優先探索
	while(!que.empty())
	{
		// キューから頂点を1つ取得し、それをキューから削除する
		int v = que.front();
		que.pop();

		for(auto &e : graph[v])  // 頂点vと接続している頂点を、1つずつeに設定
		{
			if(color[e] == 1-color[v]) continue;  // 別の色で塗り済みならOK スルー
			if(color[e] == color[v]) return false;  // 隣の頂点と同じ色のためNG

			// ここまでは矛盾なし
			// 色を塗ってキューに追加
			color[e] = 1-color[v];
			que.push(e);
		}
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
		// 未探索の頂点iを始点として探索開始
		if(!bfs(graph, i))
		{
			answer = false;  // 途中で矛盾が発生した
			break;
		}
	}

	if(answer)
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
