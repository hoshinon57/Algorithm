#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC271 https://atcoder.jp/contests/abc271

/*
 * (考え方は)動的計画法で解く。
 * 以下、0-indexedで記載する。
 * 
 * dp[i][j]を以下のように定義する。
 *   数列Eのi番目の要素までを見たときに、 (i:0～K-1)
 *   始点から都市jまでの、 (j:0～N-1)
 *   最短距離
 * (初期値の距離はINFとしておく)
 * 
 * 遷移式は、E(i)に対応する道を "a->b 距離c" としたときに、
 * b==jの場合：
 *   dp[i+1][j] = min(dp[i][j], dp[i][a]+c)
 * b!=jの場合：
 *   dp[i+1][j] = dp[i][j]
 * となる。
 * すると、dpは二次元配列にする必要はなく、添字jに相当する部分のみ持てばよい。
 * ※公式解説では「in-placeで更新可能」と書かれている
 * 
 * 以下のコードでは、一次元配列であるdist[N]を用意して実装している。
 */

struct Edge  // 1つの辺を表す
{
	int a, b, c;  // 都市a->bへ、距離c
	Edge() {}
	Edge(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}
};

int main(void)
{
	// 0-indexed
	int i;
	int N, M, K;
	cin >> N >> M >> K;
	vector<Edge> edge;
	for(i = 0; i < M; i++)
	{
		int a, b, c;
		cin >> a >> b >> c;
		a--;  // 0-indexedに変換
		b--;
		edge.push_back(Edge(a, b, c));
	}
	vector<int> e(K);
	for(i = 0; i < K; i++)
	{
		cin >> e[i];
		e[i]--;
	}
	// ここまで入力

	vector<ll> dist(N, INF64);  // dist[i]:始点から頂点iへの距離
	dist[0] = 0;  // 始点
	for(auto &m : e)  // 数列Eの先頭から順に、対応する辺を見ていく
	{
		int a, b, c;
		a = edge[m].a;
		b = edge[m].b;
		c = edge[m].c;
		dist[b] = min(dist[b], dist[a]+c);   // 都市bへの距離を更新できるか
	}

	if(dist[N-1] != INF64)
	{
		cout << dist[N-1] << endl;
	}
	else
	{
		cout << -1 << endl;
	}

	return 0;
}
