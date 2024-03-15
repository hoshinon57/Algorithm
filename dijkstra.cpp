#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ダイクストラ法のメモ、ライブラリ
// ポテンシャルについてのメモは本ファイル末尾に記載

/*
 * [関連する問題]
 * ABC342-E 難問だが勉強になった
 * ABC325-E 制約的に密グラフなので、O(V^2)のダイクストラの方が早いみたい
 * ABC305-E
 * ABC277-E ダイクストラ / 0-1BFS
 * ABC237-E ポテンシャル
 * ABC192-E
 * ABC191-E
 * ABC164-E 難問だが勉強になった
 * ABC143-E ただし想定解はワーシャルフロイド法
 * ABC142-E ただし解説はいずれもbitDPだった
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
	// 以下はAOJのGRL_1_Aを解く内容
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A&lang=ja
	int i;
	int N, E, s; cin >> N >> E >> s;
	Graph g(N);
	for(i = 0; i < E; i++)
	{
		int s, t;
		ll d;
		cin >> s >> t >> d;  // s->t
		g[s].push_back({t, d});
	}
	vector<ll> dist(N, INF64);
	dijkstra(g, s, dist);

	for(i = 0; i < N; i++)
	{
		if(dist[i] != INF64)
		{
			cout << dist[i] << endl;
		}
		else
		{
			cout << "INF" << endl;
		}
	}

	/*
	 * [ポテンシャルについて]
	 * 辺のコストに負がある場合、ダイクストラ法は使えない。
	 * ポテンシャルを用いて負の辺を消すことを考える。
	 *   https://kyo-pro.hatenablog.com/entry/ABC237E
	 *   https://niuez.hatenablog.com/entry/2019/03/04/142903
	 *   https://scrapbox.io/procon-kirokuyou/ABC237_E_-_Skiing_(500)
	 * 
	 * 頂点i->jのコストをdist(i,j)としたときに、
	 *   dist(i,j)' = dist(i,j) + p[i] - p[j]
	 * を考えてみる。
	 * pがポテンシャルであり、これを最適に定義することで負の辺を消す。
	 * 
	 * 負の辺が消えるとダイクストラ法が使えるので、頂点0を起点としたdist(0,j)'が求まる。
	 *   dist(0,j) = dist(i,j)' - p[i] + p[j]
	 * となり、本来求めたい距離が求められる。
	 * 
	 * ※
	 * ABC237-E(Skiing)では、まず楽しさの-1倍を距離として最短経路問題にする。
	 * そして標高をポテンシャルと定義した。
	 * すると下り方向にて「楽しさ＋標高」は変化しないことから、
	 *   dist(i,j)' = dist(i,j) + H[i] - H[j]
	 *   = -(H[i]-H[j]) + (H[i]-H[j]) = 0    ※H[i]>=H[j] つまり下り方向のとき
	 *     -(-2(H[j]-H[i])) + (H[i]-H[j]) = -(H[i]-H[j])  ※H[i]<H[j] つまり上り方向のとき
	 * として負の辺を消したグラフを構築できる。
	 * 本来求めたいのは dist(0,j) であり、これは
	 *   dist(0,j) = dist(0,j)' - H[0] + H[j]
	 * として求められる。
	 * 楽しさの-1倍としているので、
	 *   -(dist(0,j)' - H[0] + H[j])  の各jにおけるMAX
	 * が答となる。
	 */

	return 0;
}
