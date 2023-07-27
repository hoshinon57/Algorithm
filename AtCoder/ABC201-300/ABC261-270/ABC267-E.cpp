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
 * 二分探索で解く。
 * 
 * N回の操作を、コストXで実現可能かを考える。
 * ある値を境界として、コストXで実現不可能／実現可能が切り替わると考えると、
 *   l:コストlで操作不可能
 *   r:コストrで操作可能
 * として二分探索で境界を探せばよい。
 * 
 * ある頂点に対するコストは、減ることはあっても増えることは無いことに着目すると、
 * 「コストXで実現可能か」を判定する手順は以下。
 *   ・まず、各頂点に対してコストを求める。
 *   ・コストX以下の頂点をキューに積む。
 *   ・キューから1つ取り出す。
 *     その頂点に繋がっている頂点それぞれに対して、
 *     まだ生きている頂点であればコストを減らし、コストX以下になればキューに積む。
 *   ・キューが空になるまで続けた後、
 *     全頂点が削除済みであればOK、そうでなければNG.
 */

using Graph = vector<vector<int>>;
// Graph graph(N);
// で定義する

ll N, M;
Graph graph;
vector<ll> a;
vector<ll> cost_org;  // cost[i]:頂点iを削除するときのコスト

// 操作をコストxで可能ならtrueを返す
bool check(ll x)
{
	ll i;
	vector<ll> cost = cost_org;
	vector<bool> deleted(N, false);  // deleted[i]:頂点iが削除済みならtrue

	queue<int> que;  // コストがx以下になった頂点を積む
	for(i = 0; i < N; i++)
	{
		if(cost[i] <= x)  // コストがx以下の頂点をキューへ
		{
			que.push(i);
			deleted[i] = true;  // この時点で削除済みにしておく
		}
	}

	while(!que.empty())
	{
		int v = que.front();  // 頂点vを削除する
		que.pop();

		// 頂点vと繋がっている各頂点に対して、コストを減らす
		for(auto &e : graph[v])
		{
			if(deleted[e]) continue;  // 既に削除済みならスルー

			// 削除する頂点vだけコストを減らし、X以下になればキューに積む
			cost[e] -= a[v];
			if(cost[e] <= x)
			{
				deleted[e] = true;
				que.push(e);
			}
		}
	}

	// 全頂点が削除できたか判定
	bool all_deleted = true;
	for(i = 0; i < N; i++)
	{
		if(!deleted[i])
		{
			all_deleted = false;
			break;
		}
	}

	return all_deleted;
}

int main(void)
{
	// 0-indexed
	ll i;
	cin >> N >> M;
	graph.resize(N);
	a.resize(N);
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

	// 初期状態で各頂点を削除するコストを計算しておく
	cost_org.resize(N);
	for(i = 0; i < N; i++)
	{
		for(auto &e : graph[i])
		{
			cost_org[i] += a[e];
		}
	}

	ll l = -1;  // M=0のときコストは0になるため、左側の初期値を0にすると不正解になる
	ll r = INF64;
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid)) r = mid;
		else l = mid;
	}
	cout << r << endl;

	return 0;
}
