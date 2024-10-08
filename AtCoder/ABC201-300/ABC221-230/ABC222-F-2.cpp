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

// ABC222 https://atcoder.jp/contests/abc222

/*
 * 公式解説の、直径を用いた方針で実装してみた。
 *   https://atcoder.jp/contests/abc222/editorial/2749
 *   https://x.com/kyopro_friends/status/1446839388797763592
 * 
 * 木の直径の頂点を(s,t)とすると、頂点xから最も遠い点はs or t.
 * という典型。
 * 
 * 頂点[0,N)を入力の街、[N,2N)を街xの観光費用の頂点とする。
 * するとグラフの直径(s,t)はどちらも[N,2N)の範囲にあり、
 * 街iにおける旅費の最大値は、sおよびtからの距離の大きい方、となる。
 * 
 * ただし例外があり、i=sのときはsの観光費用の頂点は採用できない。
 * (問題文より、j!=iという式が提示されているため。また入力例2からも分かる)
 * この場合はt側の頂点となる。
 * i=tの場合も同様。
 * ※これは自明か？がピンと来なかったが、直感では合ってそう。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

// 頂点sから最も遠い頂点番号を返す
int mostfar(Graph &G, int s, vector<ll> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e.to] != INF64) continue;
			dist[e.to] = dist[v] + e.weight;
			que.push(e.to);
		}
	}
	ll w = -1;
	int ans = -1;
	for(int i = 0; i < (int)dist.size(); i++)
	{
		if(chmax(w, dist[i])) ans = i;
	}
	return ans;
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N*2);  // [0,N):入力の街 [N,2N):観光費用分
	for(i = 0; i < N-1; i++)
	{
		int a, b, c; cin >> a >> b >> c;
		a--; b--;
		g[a].push_back({b, c});
		g[b].push_back({a, c});
	}
	vector<ll> d(N);
	for(i = 0; i < N; i++) {
		cin >> d[i];
		g[i].push_back({i+N, d[i]});
		g[i+N].push_back({i, d[i]});
	}

	int s, t;  // 直径の2頂点
	vector<ll> d0(N*2, INF64), ds(N*2, INF64), dt(N*2, INF64);
	s = mostfar(g, 0, d0);
	t = mostfar(g, s, ds);
	mostfar(g, t, dt);
	// ds[x]:s->xへの距離, dt[x]:t->xの距離

	for(i = 0; i < N; i++)
	{
		// s->iとt->iのうち大きい方
		ll ans;
		if(i+N == s) ans = dt[i];  // i=sのとき、t側の値を使う
		else if(i+N == t) ans = ds[i];
		else ans = max(ds[i], dt[i]);
		cout << ans << endl;
	}

	return 0;
}
