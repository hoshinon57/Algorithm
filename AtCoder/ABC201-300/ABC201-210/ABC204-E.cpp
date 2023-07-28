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

// ABC204 https://atcoder.jp/contests/abc204

/*
 * 自力で解けず、解説を見た。
 * ダイクストラ法で解く。
 * 
 * 各頂点/辺について、どうやら (√D)-1 あたりまで待って進むのが最適らしい。
 * 「その頂点の到達時刻」が (√D)-1 以降になるケースももちろんあるので、この場合は待たない。
 * 
 * よって、ダイクストラで各辺について探索するときに、
 * √Dの前後(※)の時刻に出発するように待ったときの、次頂点に最速で到着するものを探索した。
 * (※)±10ほどとした
 * あとは通常のダイクストラ法と同様。
 * 
 * ※最初の実装ではWAやTLEが大量に出た。原因は以下。
 *   WA:「待つのはスタート地点でまとめて待てば良い、以降は待つ必要ない」と考えてしまった。おそらくこれはNG.
 *   TLE:スタート地点で待つ時間の最適値を求めるため、3分探索で何度もダイクストラを行っていた。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	ll rush;  // ラッシュD
	Edge(int to_, ll weight_, ll rush_) : to(to_), weight(weight_), rush(rush_) {}
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

		// ある頂点を時刻tに出発する場合に、次の頂点に到着する時刻を返す
		auto calc = [](ll t, ll C, ll D) -> ll
		{
			return t + C + D / (t + 1);
		};

		// 頂点vから出ている各辺について探索
		for(auto &e : G[v])
		{
			ll sqr = sqrt(e.rush);
			ll cst = calc(dist[v], e.weight, e.rush);  // 待ち時間なしで出発した場合

			// 時刻 √(e.rush) の前後(±10程度とる)の時刻に出発した場合に、次頂点に到着する時刻を求める
			// ただし時刻dist[v]より前には出発できないため、そこは除外する
			for(int i = -10; i <= 10; i++)
			{
				if(sqr+i < dist[v]) continue;  // この時刻には出発できない
				cst = min(cst, calc(sqr+i, e.weight, e.rush));
			}

//			if(dist[e.to] > dist[v] + e.weight)
			if(dist[e.to] > cst)
			{
//				dist[e.to] = dist[v] + e.weight;
				dist[e.to] = cst;
				que.push({dist[e.to], e.to});
			}
		}
	}
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b, c, d; cin >> a >> b >> c >> d;
		a--; b--;
		g[a].push_back(Edge(b, c, d));
		g[b].push_back(Edge(a, c, d));
	}

	vector<ll> dist(N, INF64);
	dijkstra(g, 0, dist);
	ll ans = -1;
	if(dist[N-1] != INF64) ans = dist[N-1];
	cout << ans << endl;

	return 0;
}
