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

// ABC325 https://atcoder.jp/contests/abc325

/*
 * https://twitter.com/kyopro_friends/status/1715728790868140239
 * を参考に、頂点を2倍にしてダイクストラ1回で解く。
 * 頂点0～N-1を車移動、N～2N-1を電車移動での頂点とし、
 * 車から電車への乗り換えは i->i+N へコスト0の辺を張る。
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
	int i, j;
	ll N, A, B, C; cin >> N >> A >> B >> C;
	Graph g(N*2);  // 0～N-1:車  N～2N-1:電車
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)  // i->j
		{
			ll d; cin >> d;
			g[i].push_back({j, d*A});
			g[i+N].push_back({j+(int)N, d*B+C});
		}
		g[i].push_back({i+(int)N, 0});
	}
	vector<ll> dist(N*2, INF64);
	dijkstra(g, 0, dist);
	cout << min(dist[N-1], dist[N*2-1]) << endl;

	return 0;
}
