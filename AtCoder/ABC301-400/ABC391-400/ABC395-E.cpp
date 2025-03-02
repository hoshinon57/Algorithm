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

// ABC935 https://atcoder.jp/contests/abc935

/*
 * 頂点倍化してダイクストラ法で解く。
 * 
 * コンテスト時はdistのキーを{頂点番号,反転状態か}で実装した。
 * この方針の実装が必要になるケースもあるので練習になったが、(ライブラリ化している)頂点倍化に比べて
 *   ・実装に時間がかかる
 *   ・実行時間もかかる (316ms vs 193ms) distを1次元か2次元かの違いかな？
 * など、頂点倍化で進める方がきっとお得。
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
	// 頂点2N (表:0～N-1 裏:N～2N-1)
	ll i;
	ll N, M, X; cin >> N >> M >> X;
	Graph g(N*2);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back({v, 1});
		g[v+N].push_back({(int)(u+N), 1});
	}
	for(i = 0; i < N; i++)
	{
		g[i].push_back({int(i+N), X});
		g[i+N].push_back({(int)i, X});
	}
	vector<ll> dist(N*2, INF64);
	dijkstra(g, 0, dist);

	cout << min(dist[N-1], dist[N*2-1]) << endl;

	return 0;
}
