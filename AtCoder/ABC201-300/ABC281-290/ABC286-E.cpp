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

// ABC286 https://atcoder.jp/contests/abc286

/*
 * ダイクストラ法で解く。
 * 
 * 頂点iを起点にダイクストラを行うにあたり、以下の2要素を情報として持たせる。
 *   {iからの距離, お土産の価値の総和を-1倍したもの}
 * 後者を-1倍しておくことで、
 *   距離が小さいほど良い
 *   距離が同じなら、価値の総和が小さいほど良い
 * となり処理が楽になる。
 * 
 * ダイクストラ1回あたりの計算量は、辺数をEとして O(ElogN). 密グラフがありえるので O(N^2*logN).
 * クエリがN^2回あるので、事前に各頂点からのダイクストラを行っておく必要がある(=N回)。
 * ⇒これにより、全体の計算量は O(N*N^2*logN) -> O(N^3*logN) となる。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;
using pll = pair<ll,ll>;

// a:入力のお土産の価値
void dijkstra(Graph &G, int s, vector<pll> &dist, vector<ll> &a)
{
	dist[s] = {0, a[s]};  // 始点sでもお土産を買う
	using dist_v = pair<pll, int>;  // 距離とその頂点番号のpair
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;
	que.push({dist[s], s});

	while(!que.empty())
	{
		pll di = que.top().first;
		int v = que.top().second;
		que.pop();

		if(di > dist[v]) continue;  // キューのデータはゴミのためスルー

		// 頂点vから出ている各辺について探索
		for(auto &e : G[v])
		{
			pll d_next;
			d_next.first = dist[v].first + e.weight;
			d_next.second = dist[v].second + a[e.to];
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
	// 0-indexed
	int i, j;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i]; a[i] = -a[i];}  // -1倍して登録
	Graph g(N);
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		for(j = 0; j < N; j++)
		{
			if(s[j] == 'Y')  // i->jへの辺
			{
				g[i].push_back({j, 1});
			}
		}
	}

	// dist[i][j]:iを起点としてダイクストラしたときの、i->jへの距離
	vector<vector<pll>> dist_mas(N, vector<pll>(N, {INF64, INF64}));  // dist_mas[N][N]
	for(i = 0; i < N; i++)  // 頂点iを起点にダイクストラ
	{
		dijkstra(g, i, dist_mas[i], a);
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int u, v; cin >> u >> v;
		u--; v--;
		if(dist_mas[u][v] == make_pair(INF64, INF64))
		{
			cout << "Impossible" << endl;
		}
		else
		{
			auto [l,val] = dist_mas[u][v];
			cout << l << " " << -val << endl;
		}
	}

	return 0;
}
