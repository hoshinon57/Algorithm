#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// code13.3の写経と改善
// 本のコードはbfs()にて引数sを使っていない(常に0を始点としている)バグがある。
// 以下のコードではそれを修正済み。

using Graph = vector<vector<int>>;

int N, M;  // N:頂点数 M:辺の数
vector<int> dist;  // bfs()で指定される始点sからの距離  未訪問orたどれない頂点は-1

// 頂点sから各頂点への最短路長を計算してdist[]に設定する
void bfs(Graph &graph, int s)
{
	dist.assign(N, -1);  // 全頂点を未訪問として初期化
	queue<int> que;

	// 頂点sを始点とする
	dist[s] = 0;
	que.push(s);

	// 幅優先探索
	while(!que.empty())
	{
		// キューから頂点を1つ取得し、それをキューから削除する
		int v = que.front();
		que.pop();

		for(auto &e : graph[v])  // 頂点vと接続している頂点を、1つずつeに設定
		{
			// queueを使うロジック上、dist[e]に対して後から設定する方がより短くなる、というケースは存在しない。
			// よって先優先で設定してしまって問題ない。

			if(dist[e] != -1) continue;  // 探索済みならスルー

			// 距離を設定してキューに積む
			dist[e] = dist[v]+1;
			que.push(e);
		}
	}
}

int main(void)
{
	// 以下、0-indexedかつ無向グラフ
	int i;
	cin >> N >> M;

	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}

	// 頂点0を始点とし、幅優先探索にて最短路長を求める
	bfs(graph, 0);

	for(i = 0; i < N; i++)
	{
		cout << i << ":" << dist[i] << endl;
	}

	return 0;
}
