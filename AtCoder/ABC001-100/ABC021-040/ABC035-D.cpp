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

// ABC035 https://atcoder.jp/contests/abc035

/*
 * ダイクストラ法で解く。
 * 
 * ちょっと検討すると、滞在する町は1つで十分だと分かる。複数の町で滞在してもより良い答にはならない。
 * よって滞在する町xを固定して、
 *   1->xへの移動時間 + (可能な限り)町xに滞在 + x->1への移動時間
 * での所持金の最大が答になる。
 * 
 * 1->xへの移動時間は入力そのままのグラフで、
 * x->1は入力を反転したグラフで、それぞれ求められる。
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
	ll i;
	ll N, M, T; cin >> N >> M >> T;
	vector<ll> aa(N); for(i = 0; i < N; i++) {cin >> aa[i];}
	Graph g(N), rg(N);
	for(i = 0; i < M; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		a--; b--;
		g[a].push_back({(int)b, c});
		rg[b].push_back({(int)a, c});  // 反転グラフ
	}

	vector<ll> di(N, INF64), rdi(N, INF64);
	dijkstra(g, 0, di);
	dijkstra(rg, 0, rdi);

	ll ans = 0;
	for(i = 0; i < N; i++)  // 滞在する町i
	{
		ll tmp = max(0LL, T - di[i] - rdi[i]) * aa[i];  // 移動時間の残りを滞在にあてるとして、どれだけ稼げるか
		chmax(ans, tmp);
	}
	cout << ans << endl;

	return 0;
}
