#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC267 https://atcoder.jp/contests/abc267

/*
 * 優先度付きキュー＆貪欲法で解く。
 * 
 * 現在残っている頂点のうち、最小コストのものを順に削除していく。
 * 最小コストである頂点を取得するには、優先度付きキューを用いる。
 * キューは {コスト, 頂点番号} の形で保持する。
 * 
 * 処理の概要は以下。
 * ・まず、全頂点に対してコストを計算し、優先度付きキューに積む。
 * ・キューから1つ取り出す。これはコストが最小のものであり、この頂点を削除することになる。
 *   このコストの最大値を保持しておく。
 *   頂点vを削除するので、
 *   頂点vと繋がっている頂点に対して、頂点vの数値ぶんコストを減らし、キューに積む。
 * ・キューが空になるまで続けた後、
 *   保持していたコスト最大値が答となる。
 */

using Graph = vector<vector<int>>;
// Graph graph(N);
// で定義する

int main(void)
{
	// 0-indexed
	ll i;
	ll N, M;
	cin >> N >> M;
	Graph graph(N);
	vector<ll> a(N);
	for(i = 0; i < N; i++) cin >> a[i];
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;  // 0-indexedに変換
		v--;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	// ここまで入力

	priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> que;  // {cost, v_no}
	vector<ll> cost(N);  // cost[i]:頂点iを削除するコスト
	for(i = 0; i < N; i++)
	{
		for(auto &e : graph[i])
		{
			cost[i] += a[e];
		}
		que.push({cost[i], i});
	}

	vector<bool> deleted(N, false);  // deleted[i]:頂点iが削除済みならtrue
	ll answer = -1;
	while(!que.empty())
	{
		ll v = que.top().second;
		ll v_cost = que.top().first;
		que.pop();

		if(deleted[v]) continue;  // 削除済みならスルー

		// 頂点vを削除する
		// その際のコストの最大値を保持しておく
		deleted[v] = true;
		answer = max(answer, v_cost);

		// 頂点vを削除するので、
		// 頂点vと繋がっている頂点に対して、頂点vの数値ぶんコストを減らす
		for(auto &e : graph[v])
		{
			if(deleted[e]) continue;  // 削除済みならスルー
			cost[e] -= a[v];
			que.push({cost[e], e});
		}
	}
	cout << answer << endl;

	return 0;
}
