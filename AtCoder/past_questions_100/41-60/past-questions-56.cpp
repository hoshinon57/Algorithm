#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題56
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A&lang=ja

/*
 * ヒープを用いたダイクストラ法で解く。
 * いわゆる「教科書通りの実装」であり、説明は省略する。
 */

// 1つの辺を表す構造体
struct Edge {
	ll to;  // 辺の行き先
	ll w;  // 辺の重み
	Edge(ll to_, ll w_) : to(to_), w(w_) {};
};

using Graph = vector<vector<Edge>>;

int main(void)
{
	ll i;
	ll V, E;
	ll r;
	cin >> V >> E >> r;
	Graph graph(V);
	for(i = 0; i < E; i++)
	{
		ll s, t, d;
		cin >> s >> t >> d;
		graph[s].push_back(Edge(t, d));  // s->tへ重みdの辺
	}

	vector<ll> dist(V, INF64);  // dist[i]:始点から頂点iへの距離
	using pll = pair<ll, ll>;  // {ある頂点への距離, その頂点番号}
	// pair<ll,ll>型の要素を持ち、小さい方が優先度が高いキュー
	priority_queue<pll, vector<pll>, greater<pll>> que;

	// 初期設定 rが始点
	dist[r] = 0;
	que.push({dist[r], r});

	while(!que.empty())
	{
		ll d = que.top().first;
		ll v = que.top().second;
		que.pop();

		if(d > dist[v]) continue;  // キューから取り出した値から、さらに更新済み

		for(auto &e : graph[v])  // e:頂点vからの辺
		{
			if(dist[e.to] > dist[v] + e.w)  // 頂点e.toへの距離を更新できるか
			{
				dist[e.to] = dist[v] + e.w;
				que.push({dist[e.to], e.to});
			}
		}
	}

	for(i = 0; i < V; i++)
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

	return 0;
}
