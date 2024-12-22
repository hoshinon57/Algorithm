#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// AOJ 2878
// https://onlinejudge.u-aizu.ac.jp/challenges/sources/VPC/RUPC/2878?year=2018

/*
 * 問題文が微妙だが、辺は双方向と思って良さそう。
 * 3点a,b,cのうち2点を選んで辺を張るとのことだが、仮に3点でそれぞれ辺を張ったとしても
 * 最短経路がa->b->cのように3点を通るケースって無くない？となった。
 * ということで、3点で双方向に辺を張ってダイクストラ法をやって良さそう、と思って実装してAC.
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
			ll d_next;
			// 頂点vと辺eを用いた先の頂点について、距離を求める。
			// 基本はdist[v]+e.weightで良いが、問題によっては＋αが必要。(ABC192-E,ABC204-E等)
			d_next = dist[v] + e.weight;
			if(dist[e.to] > d_next)
			{
				dist[e.to] = d_next;
				que.push({dist[e.to], e.to});
			}
		}
	}
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	int s, t; cin >> s >> t;
	s--; t--;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b, c, d; cin >> a >> b >> c >> d;
		a--; b--; c--;
		g[a].push_back({b, d});
		g[a].push_back({c, d});
		g[b].push_back({a, d});
		g[b].push_back({c, d});
		g[c].push_back({a, d});
		g[c].push_back({b, d});
	}
	vector<ll> dist(N, INF64);
	dijkstra(g, s, dist);
	cout << dist[t] << endl;

	return 0;
}
