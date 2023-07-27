#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC277 https://atcoder.jp/contests/abc277

/*
 * ABC277-E-1-BFS(NG).cppのTLE解消版。
 * ダイクストラ法で解く。
 * 
 * 頂点数を2倍の2N個とし、
 *   (1)頂点0～N-1：スイッチ初期状態におけるグラフ用
 *   (2)頂点N～2N-1：スイッチ反転状態におけるグラフ用
 * と定義する。
 * 
 * 入力のu,v,aにおいて、a=1なら(1)側の、a=0なら(2)側で、辺の重みを1としてグラフを構築する。
 * またスイッチがある頂点は、iとi+Nの2頂点を、こちらは重み0でグラフ構築する。
 * 
 * あとは通常のダイクストラ法で解けばよい。
 * 答は、頂点N-1と2N-1のうち小さい方となる。(ゴールに達した際、スイッチの状態は関係ないので)
 */

// 辺を表す構造体
struct Edge
{
	int to;  // 行き先
	int w;  // 重み
	Edge(int _to, int _w) : to(_to), w(_w) {}
};

int main(void)
{
	// 0-indexed
	int i;
	int N, M, K;
	cin >> N >> M >> K;
	// 頂点数を2倍の2N個として処理する
	//   (1)頂点0～N-1：スイッチ初期状態におけるグラフ用
	//   (2)頂点N～2N-1：スイッチ反転状態におけるグラフ用
	using Graph = vector<vector<Edge>>;
	Graph g(2*N);
	for(i = 0; i < M; i++)
	{
		int u, v, a;
		cin >> u >> v >> a;
		u--; v--;
		if(a == 1)
		{
			g[u].push_back({v, 1});
			g[v].push_back({u, 1});
		}
		else
		{
			g[u+N].push_back({v+N, 1});
			g[v+N].push_back({u+N, 1});
		}
	}
	for(i = 0; i < K; i++)
	{
		int s;
		cin >> s;
		s--;
		// 頂点iとi+Nはコスト0で繋がっている
		g[s].push_back({s+N, 0});
		g[s+N].push_back({s, 0});
	}

	vector<ll> dist(2*N, INF64);
	dist[0] = 0;  // 始点

	using pli = pair<ll, int>;  // {頂点までの最短距離, その頂点番号}
	priority_queue<pli, vector<pli>, greater<pli>> que;  // pairの第1要素が小さい方が、優先度が高い
	que.push({0, 0});
	
	while(!que.empty())
	{
		ll d = que.top().first;
		int v = que.top().second;
		que.pop();

		if(d > dist[v]) continue;  // 既により小さい値で更新済み

		for(auto &e : g[v])  // e:頂点vから出ている辺
		{
			if(dist[e.to] > dist[v] + e.w)
			{
				dist[e.to] = dist[v] + e.w;
				que.push({dist[e.to], e.to});
			}
		}
	}

	// スイッチ状態2通りそれぞれにおける頂点Nまでの距離のうち、小さい方
	ll answer = min(dist[N-1], dist[2*N-1]);
	if(answer != INF64)
	{
		cout << answer << endl;
	}
	else
	{
		cout << -1 << endl;
	}

	return 0;
}
