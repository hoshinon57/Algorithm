#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題13.5

/*
 * 入次数が0である頂点を探し、トポロジカルソートに追加した後、その頂点および辺を削除する。
 * 削除は、削除する頂点から繋がっている頂点の入次数を減らすことで実現する。
 * これを全ての頂点が無くなるまで繰り返す。
 * 
 * 参考：https://qiita.com/Morifolium/items/6c8f0a188af2f9620db2
 */

using Graph = vector<vector<int>>;

int main(void)
{
	// 以下、0-indexedかつ有向グラフ
	int i;
	int N, M;  // 頂点の数、辺の数
	cin >> N >> M;

	Graph graph(N);
	vector<int> graph_in(N);  // graph_in[i]:頂点iの入次数(入ってくる辺の数)
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph_in[b]++;  // 頂点bへの入力
	}

	queue<int> que;
	vector<int> order;  // トポロジカルソート順
	// 初期設定として、入次数が0の頂点を探してキューに積む
	for(i = 0; i < N; i++)
	{
		if(graph_in[i] == 0)
		{
			que.push(i);
		}
	}

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		order.push_back(v);  // トポロジカルソートに追加

		// 頂点vを消したいので、vに繋がっている頂点を列挙する
		for(auto &e : graph[v])
		{
			graph_in[e]--;  // 頂点vを消すので、その先の頂点の入次数を1減らす
			if(graph_in[e] == 0)  // 入次数が0になったらキューへ積む
			{
				que.push(e);
			}
		}
	}

	for(auto &e : order)
	{
		cout << e << ", ";
	}
	cout << endl;

	return 0;
}
