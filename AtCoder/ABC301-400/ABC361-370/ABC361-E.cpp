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

// ABC361 https://atcoder.jp/contests/abc361

// 解説の通りで、木の直径をDとして (ΣC*2)-D が答。
// 直径は最短距離計算を2回やる[典型]

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

int dijkstra(Graph &G, int s, vector<ll> &dist)
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

	return max_element(dist.begin(), dist.end()) - dist.begin();
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	ll sum = 0;
	for(i = 0; i < N-1; i++)
	{
		int a, b, c; cin >> a >> b >> c;
		a--; b--;
		g[a].push_back({b,c});
		g[b].push_back({a,c});
		sum += c;
	}
	sum *= 2;
	
	vector<ll> dist1(N, INF64), dist2(N, INF64);
	int s = dijkstra(g, 0, dist1);
	int t = dijkstra(g, s, dist2);
	// s-tが直径
	sum -= dist2[t];
	cout << sum << endl;

	return 0;
}
