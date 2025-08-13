#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 2025/8の再解き時にソースコードを置き換えた

// ABC277 https://atcoder.jp/contests/abc277

/*
 * ABC277-E-1-BFS(NG).cppのTLE解消版。
 * ダイクストラ法で解く。
 * 
 * 頂点数を2倍の2N個とし、
 *   (1)頂点0～N-1：スイッチ初期状態におけるグラフ用
 *   (2)頂点N～2N-1：スイッチ反転状態におけるグラフ用
 * と定義する。
 * 
 * 入力のu,v,aにおいて、a=1なら(1)側の、a=0なら(2)側で、辺の重みを1としてグラフを構築する。
 * またスイッチがある頂点は、iとi+Nの2頂点を、こちらは重み0でグラフ構築する。
 * 
 * あとは通常のダイクストラ法で解けばよい。
 * 答は、頂点N-1と2N-1のうち小さい方となる。(ゴールに達した際、スイッチの状態は関係ないので)
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
	ll i;
	ll N, M, K; cin >> N >> M >> K;
	Graph g(N*2);
	for(i = 0; i < M; i++)
	{
		ll u, v, a; cin >> u >> v >> a;
		u--; v--;
		if(a == 0)
		{
			u += N;
			v += N;
		}
		g[u].push_back({(int)v, 1});
		g[v].push_back({(int)u, 1});
	}
	vector<ll> s(K); for(i = 0; i < K; i++) {
		cin >> s[i];
		s[i]--;
		g[s[i]].push_back({(int)(s[i]+N), 0});
		g[s[i]+N].push_back({(int)s[i], 0});
	}
	vector<ll> dist(N*2, INF64);
	dijkstra(g, 0, dist);
	ll ans = min(dist[N-1], dist[N*2-1]);
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
