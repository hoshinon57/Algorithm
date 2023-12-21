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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC143 https://atcoder.jp/contests/abc143

/*
 * けんちょん氏の解説を元に、ダイクストラ法で解いた。
 *   https://drken1215.hatenablog.com/entry/2019/10/20/171500
 * 
 * 距離distを {補給回数, 前回補給から使った燃料} として定める。 
 * 全頂点それぞれを始点にしてダイクストラ法を行う。
 * 後は実装を参照。
 * 
 * 同じ型のpairなら <, > が使えるので、通常のダイクストラのライブラリからそれほど変えずに実装できた。
 */

using pll = pair<ll,ll>;  // dist:{補給回数, 前回補給から使った燃料}
ll L;
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
void dijkstra(Graph &G, int s, vector<pll> &dist)
{
	dist[s] = {0, 0};
	using dist_v = pair<pll, int>;  // 距離とその頂点番号のpair
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;
	que.push({dist[s], s});

	while(!que.empty())
	{
//		ll hokyu = que.top().first.first;
//		ll use = que.top().first.second;
		pll d = que.top().first;
		int v = que.top().second;
		que.pop();

//		if(hokyu > dist[v].first) continue;
//		if(hokyu == dist[v].first && use > dist[v].second) continue;
		// ★同じ型のpairは比較できる
		if(d > dist[v]) continue;

		// 頂点vから出ている各辺について探索
		for(auto &e : G[v])
		{
			pll d_next;
			if(e.weight > L) continue;  // そもそもこの道を使えない
			// 頂点vと辺eを用いた先の頂点について、距離を求める
			if(dist[v].second + e.weight <= L)  // 補給不要
			{
				d_next.first = dist[v].first;
				d_next.second = dist[v].second + e.weight;
			}
			else  // 補給必要
			{
				d_next.first = dist[v].first + 1;
				d_next.second = e.weight;
			}
//			if((dist[e.to].first > d_next.first)
//				|| (dist[e.to].first == d_next.first && dist[e.to].second > d_next.second) )
			// ★同じ型のpairは比較できる
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
	ll N, M; cin >> N >> M >> L;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		ll c;
		cin >> a >> b >> c;
		a--; b--;
		g[a].push_back({b, c});
		g[b].push_back({a, c});
	}
	vector<vector<pll>> dist_master(N, vector<pll>(N));
	for(i = 0; i < N; i++)  // 頂点iを起点にダイクストラ
	{
		vector<pll> dist(N, {INF64,INF64});
		dijkstra(g, i, dist);
		dist_master[i] = dist;
	}
	int Q; cin >> Q;
	while(Q > 0)
	{
		int s, t; cin >> s >> t;
		s--; t--;
		ll ans = dist_master[s][t].first;
		if(ans == INF64) ans = -1;
		cout << ans << endl;
		Q--;
	}

	return 0;
}
