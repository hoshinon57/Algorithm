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

// ABC325 https://atcoder.jp/contests/abc325

/*
 * ダイクストラ法で解く。
 * 
 * ある都市に2回以上立ち寄る必要はないことを考えると、
 * 各都市iについて
 *   都市1⇒iへ社用車で行く場合の時間 X
 *   都市i⇒Nへ電車で行く場合の時間 Y
 * の和X+Yが、都市iにて電車に乗り換えた場合の最短時間となる。
 * これを全都市について計算したもののうち、最小のものが答となる。
 * 
 * Xは都市1からのダイクストラで、
 * Yは都市Nからのダイクストラでそれぞれ求められる。
 * 
 * ヒープを使った場合のダイクストラ法の計算量はO(ElogV)であり、
 * 計算量はO(N^2*logN)となる。
 * 本問の入力は密グラフであるので、「ヒープじゃない方」のダイクストラ O(V^2) の方が早いとのこと。
 *   https://www.youtube.com/watch?v=KiujjWaQJpk
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
	Graph g1(N), g2(N);
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)  // i->j
		{
			ll d; cin >> d;
			g1[i].push_back(Edge(j, d*A));  // 社用車
			g2[i].push_back(Edge(j, d*B+C));  // 電車
		}
	}

	vector<ll> dist1(N, INF64), dist2(N, INF64);
	dijkstra(g1, 0, dist1);  // 都市1から社用車で行く場合の最短距離
	dijkstra(g2, N-1, dist2);  // 都市Nから電車

	ll ans = INF64;
	for(i = 0; i < N; i++)
	{
		ll tmp = dist1[i] + dist2[i];
		ans = min(ans, tmp);
	}
	cout << ans << endl;

	return 0;
}
