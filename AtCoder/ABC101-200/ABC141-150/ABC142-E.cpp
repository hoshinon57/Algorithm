#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC142 https://atcoder.jp/contests/abc142

/*
 * ダイクストラ法で解く。
 * 
 * 各宝箱について、開いている/開いていないをbit(1 or 0)で管理し、これを頂点とする。
 * 頂点数は2^N通り(0～2^N-1)となる。
 * 
 * 頂点xからある鍵を使うことを考える。
 * この鍵で開けられる宝箱を、bitごとに開けられる/開けられないで同様に表現すると、これをcとして
 *   遷移：x -> x|c
 *   コスト：a円
 * となる。
 * 
 * すると、
 *   頂点数：2^N
 *   辺数：2^N * M
 * となるグラフが構築できるので、頂点0からダイクストラ法を実施し、頂点(2^N-1)までの距離が答となる。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;
// 頂点sを起点にダイクストラ。
// distは未探索部分をINFで埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void dijkstra(Graph &G, int s, vector<ll> &dist)
{
	dist[s] = 0;
	using dist_v = pair<ll, int>;  // 距離とその頂点番号のpair
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;
	que.push({dist[s], s});

	while(!que.empty())
	{
		ll d = que.top().first;
		int v = que.top().second;
		que.pop();

		if(d > dist[v]) continue;  // {d, v}はゴミのためスルー

		// 頂点vから出ている各辺について探索
		for(auto &e : G[v])
		{
			if(dist[e.to] > dist[v] + e.weight)
			{
				dist[e.to] = dist[v] + e.weight;
				que.push({dist[e.to], e.to});
			}
		}
	}
}

int main(void)
{
	// 0-indexed
	int i, j;
	int N, M; cin >> N >> M;
	Graph g(1<<N);  // 頂点数:2^N  頂点番号:0～(2^N)-1
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		int c = 0;  // この鍵で開けられる宝箱をbitで表現
		for(j = 0; j < b; j++)
		{
			int c_tmp; cin >> c_tmp;
			c_tmp--;
			c |= (1<<c_tmp);
		}
		for(j = 0; j < 1<<N; j++)
		{
			// 頂点jからj|cへ、コストaの辺
			g[j].push_back(Edge(j|c, a));
		}
	}

	vector<ll> dist(1<<N, INF64);
	dijkstra(g, 0, dist);
	ll ans = dist[(1<<N)-1];  // 全bitが1になっている頂点
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
