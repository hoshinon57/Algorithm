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

// ABC191 https://atcoder.jp/contests/abc191

/*
 * ダイクストラ法で解く。
 * 
 * N,Mがともに最大2000であるため、各頂点ごとにダイクストラ法を用いても十分間に合う。
 * ただし始点と終点が同じ町となる条件がネック。
 * 
 * そのため以下のようにした。
 *   頂点を2倍にし、0～N-1は通常の頂点、N～2N-1は「スタート専用」の頂点とする。
 *   入力A->Bを、A->Bおよび(A+N)->Bの2本の辺として扱う。
 *   頂点iについて、i+Nを始点, iを終点としてダイクストラを用いる。
 * 以上で正しく判定できる。
 * 
 * 上記解法で解いた後、WEB上の解説を参照して
 *   「道路の向きを反転して2回ダイクストラ」「スタート地点の初期コストをINFにする」
 * などを見つけたが、自分にはイマイチ分かりづらく感じた。
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
	int i;
	int N, M; cin >> N >> M;
	Graph g(N*2);
	for(i = 0; i < M; i++)
	{
		int a, b, c; cin >> a >> b >> c;
		a--; b--;
		g[a  ].push_back({b, c});  // a->b
		g[a+N].push_back({b, c});  // a(スタート専用)->b
	}

	for(i = 0; i < N; i++)
	{
		// i+Nを始点としてダイクストラ
		// iが終点
		vector<ll> dist(N*2, INF32);
		dijkstra(g, i+N, dist);
		ll ans = -1;
		if(dist[i] != INF32) ans = dist[i];
		cout << ans << endl;
	}

	return 0;
}
