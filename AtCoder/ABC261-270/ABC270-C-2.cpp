#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC270 https://atcoder.jp/contests/abc270

/*
 * 幅優先探索で解く。
 * 
 * まずは、頂点Xを始点としたときの各頂点への距離を幅優先探索で求め、dist[]に設定する。
 * するとX->Yへのパスを求めるには、ゴールであるYからdist[]が1ずつ小さくなるように
 * 頂点をたどっていけばよい。
 * 
 * 深さ優先探索での解法(ABC270-C.cpp)と同じく、
 * 頂点のリストはゴールからスタート方向へ逆順となっているので、
 * 逆順に出力するか、reverse()にて反転させる必要がある。
 */

using Graph = vector<vector<int>>;

vector<int> ans_list;  // X->Yへのパス上の頂点を列挙

// 頂点v->tへのパスを幅優先探索で求める
// dist[i]:頂点Xから頂点iへの距離 探索時に更新していく
void bfs(Graph &g, int X, int Y, vector<int> &dist)
{
	queue<int> que;

	// 始点は距離0にし、キューに積む
	dist[X] = 0;
	que.push(X);

	// キューから1つずつ取り出しながら、そこと繋がる頂点の距離を求めていく
	while(!que.empty())
	{
		int v = que.front();
		que.pop();

		for(auto &e : g[v])
		{
			if(dist[e] > dist[v] + 1)  // 距離が更新できるか（＝逆流防止）
			{
				dist[e] = dist[v] + 1;
				que.push(e);
			}
		}
	}

	// パス上の頂点をans_list[]に列挙する
	int v = Y;  // ゴールからスタート方向へ
	while(true)
	{
		ans_list.push_back(v);
		if(v == X) break;  // スタートに到達したら終了
		for(auto &e : g[v])
		{
			// 次に行くべき頂点は、スタートからの距離が1短いものとなる
			if(dist[e] == dist[v] - 1)
			{
				v = e;
				break;
			}
		}
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N, X, Y;
	cin >> N >> X >> Y;
	X--;  // 0-indexedに変換
	Y--;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;  // 0-indexedに変換
		v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	// X->Yのパスを求めて、パス上の頂点をans_list[]に列挙する
	vector<int> dist(N, INF32);  // dist[i]:頂点Xから頂点iへの距離
	bfs(g, X, Y, dist);

	// Y->Xの方向に並んでいるので、反転する
	std::reverse(ans_list.begin(), ans_list.end());

	// 結果出力
	for(auto &e : ans_list)
	{
		cout << e+1 << " ";  // 1-indexedに戻すため+1
	}
	cout << endl;

	return 0;
}
