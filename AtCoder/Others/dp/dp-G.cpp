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
 * 幅優先探索を用いたトポロジカルソートで解く。
 * 
 * 「有向閉路を含まない」という条件から、
 * トポロジカルソートを使って先頭から距離を確定させていく。
 * 
 * 幅優先探索でのトポロジカルソートの構築手順は以下。
 *   入次数が0である頂点を探し、トポロジカルソートに追加した後、その辺を削除する。
 *   (削除は、削除する頂点から繋がっている頂点の入次数を減らすことで実現する)
 *   これを全ての頂点が無くなるまで繰り返す。
 * 入次数が0になった頂点について距離を確定できる。
 */
using Graph = vector<vector<int>>;

int main(void)
{
	// 0-indexedかつ重み無し有向グラフ
	int i;
	int N, M;
	cin >> N >> M;
	Graph graph(N);
	vector<int> graph_in(N);  // graph_in[i]:頂点iの入次数
	for(i = 0; i < M; i++)
	{
		int x, y;
		cin >> x >> y;
		x--;  // 0-indexedに変換
		y--;
		graph[x].push_back(y);
		graph_in[y]++;  // 頂点yへの入力
	}

	vector<int> dist(N, -1);  // dist[i]:頂点iまでの最長パス
	queue<int> que;
	// 初期設定として、入次数が0の頂点を探してキューに積む
	for(i = 0; i < N; i++)
	{
		if(graph_in[i] == 0)
		{
			dist[i] = 0;  // 始点は距離0
			que.push(i);
		}
	}

	while(!que.empty())
	{
		int v = que.front();
		que.pop();

		for(auto &e : graph[v])
		{
			graph_in[e]--;  // 頂点vから出る辺を削除するイメージ
			// 頂点eの入次数が0になったら、距離を確定してキューに積む
			if(graph_in[e] == 0)
			{
				// 幅優先探索のため、ここまでの距離が小さいものから順に探索される。
				// よってdist[e]の更新時にmax()を使う必要はなく、無条件で設定してしまって良い。
				dist[e] = dist[v]+1;
				que.push(e);
			}
		}
	}

	int answer = -1;
	for(auto &e : dist)
	{
		answer = max(answer, e);
	}
	cout << answer << endl;
	
	return 0;
}
