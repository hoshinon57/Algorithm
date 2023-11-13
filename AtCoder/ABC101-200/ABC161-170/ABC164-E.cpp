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

// ABC164 https://atcoder.jp/contests/abc164

/*
 * 自力で解けず、解説を見た。
 * ダイクストラ法で解く。
 *   https://drken1215.hatenablog.com/entry/2020/04/27/190000
 *   https://zenn.dev/efef/scraps/3a51268632d7f3#comment-45acb1a459d3af
 *   https://kmjp.hatenablog.jp/entry/2020/04/28/0900
 *   https://blog.hamayanhamayan.com/entry/2020/04/26/232132
 * 
 * まず、金貨は無限にあると考えて良い。
 * N=50, A=50という制約から、銀貨を50*50=2500枚持っておけば、これ以上の補充をせずにどの都市へも行くことができる。
 * よって銀貨が2500枚より多い分については、捨ててしまって良い。(2500枚は必要である点には注意)
 * 
 * とすると、{都市,所持銀貨}の組み合わせで頂点を作ることで、ダイクストラ法で解くことができる。
 * {0,0},{0,1},...,{0,2500},{1,0},{1,1},...,{N-1,2500} のN*2500個の頂点になるイメージ。
 * 都市u,v間の移動については以下となる。
 *   {u,s} -> {v,s-a} へコストbの辺
 *   {v,s} -> {u,s-a} へコストbの辺
 * また都市uでの銀貨交換は以下となる。
 *   {u,s} -> {u,s+c} へコストdの辺
 * 
 * 上記の辺を張ってダイクストラを実施すると、
 * 都市tについての答は {t,0},{t,1},...,{t,2500} の最小値となる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・ある都市へ2回以上来るケースはあるか？
 *   ⇒ある。他の都市で銀貨を補充して戻ってくるようなケース。
 *   ある都市にて所持銀貨がX枚であるような状態で、2回以上来るケースはあるか？
 *   ⇒これは無さそう。
 * ・N=50, A=50という制約が気になる。
 *   それぞれ都市の数、都市間のコストを踏まえると、
 *   銀貨を50*50=2500枚持っておけば、これ以上の補充をせずにどの都市へも行くことができる。
 * ・すると状態としては{都市,所持銀貨}の組み合わせになりそう。
 *   これを頂点としてダイクストラをすれば良さそう。
 */

// 頂点を結ぶ辺
struct Edge
{
	ll to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(ll to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

const ll SILVMAX = 2505;  // これ以上の銀貨は不要
// 頂点v, 所持銀貨mのときの頂点番号を返す
ll conv(ll v, ll m)
{
	return v*SILVMAX + m;
}

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
	ll i, j;
	ll N, M, S; cin >> N >> M >> S;
	S = min(S, SILVMAX-1);  // これ以上は不要
	Graph g(N*SILVMAX);
	for(i = 0; i < M; i++)
	{
		ll u, v, a, b; cin >> u >> v >> a >> b;
		u--; v--;
		for(j = 0; j < SILVMAX; j++)  // 頂点u, 所持jから頂点v, 所持j-aへ移動 コストb
		{
			if(j-a < 0) continue;  // 所持銀貨足りない
			// {頂点, 所持銀貨}で表したときに、
			// {u,j} -> {v,j-a} へコストb
			ll uu = conv(u, j);
			ll vv = conv(v, j-a);
			g[uu].push_back({vv, b});
			// {v,j} -> {u,j-a} へコストb
			vv = conv(v, j);
			uu = conv(u, j-a);
			g[vv].push_back({uu, b});
		}
	}
	for(i = 0; i < N; i++)  // 都市iにて銀貨交換
	{
		ll c, d; cin >> c >> d;
		for(j = 0; j < SILVMAX; j++)
		{
			// {i,j} -> {i,j+c} へコストb ただしj+cは(SLIVMAX-1)まで
			ll uu = conv(i, j);
			ll vv = conv(i, min(j+c, SILVMAX-1));
			g[uu].push_back({vv, d});
		}
	}

	vector<ll> dist(N*SILVMAX, INF64);
	dijkstra(g, conv(0, S), dist);

	for(i = 1; i < N; i++)  // 都市iについて、銀貨を0～(SILVMAX-1)枚まで持つ中での最小値を求める
	{
		ll ans = INF64;
		for(j = 0; j < SILVMAX; j++)
		{
			ans = min(ans, dist[conv(i, j)]);
		}
		cout << ans << endl;
	}

	return 0;
}
