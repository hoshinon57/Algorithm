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

// ABC022 https://atcoder.jp/contests/abc022

/*
 * ダイクストラ法で解いた。
 * 
 * 家0と隣接する家xについて、
 * 家0-e間の道を除外したマップにて、e->0への最短距離を求めると、それと除外した道の和がxについての答となる。
 * これを家0と隣接する各家について求めて、その最小値が答。
 * 
 * 計算量は、
 * ダイクストラ法が1回あたりO(N^2logN)になるので、全体でO(N^3logN)か。
 * ちょっとギリギリだったかな。(416ms)
**/

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
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> u(M), v(M);
	vector<ll> l(M);
	for(i = 0; i < M; i++)
	{
		cin >> u[i] >> v[i] >> l[i];
		u[i]--; v[i]--;
		if(u[i] > v[i]) swap(u[i], v[i]);
	}

	// 家0-xを結ぶ辺idxについて、
	// それを使わない辺でグラフを構築し、x->0への距離を返す
	auto calc = [&](int idx) -> ll
	{
		Graph g(N);
		for(j = 0; j < M; j++)
		{
			if(j == idx) continue;
			g[u[j]].push_back({v[j], l[j]});
			g[v[j]].push_back({u[j], l[j]});
		}
		vector<ll> dist(N, INF64);
		dijkstra(g, v[idx], dist);
		return dist[0];
	};

	ll ans = INF64;
	for(i = 0; i < M; i++)
	{
		if(u[i] == 0)
		{
			ll tmp = calc(i);
			tmp += l[i];
			chmin(ans, tmp);
		}
	}
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
