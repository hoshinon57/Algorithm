#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題60
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_C&lang=ja

/*
 * ワーシャルフロイド法で解く。
 * いわゆる「教科書通りの実装」であり、説明は省略する。
 * 計算量はO(V^3).
 */

int main(void)
{
	int i, j, k;
	int V, E;
	cin >> V >> E;
	vector<vector<ll>> dist(V, vector<ll>(V, INF64));  // dist[i][j]:i->jへの最短距離
	for(i = 0; i < E; i++)
	{
		int s, t;
		ll d;
		cin >> s >> t >> d;  // s->t 距離d
		dist[s][t] = d;
	}
	for(i = 0; i < V; i++) dist[i][i] = 0;  // 同じ点どうしは距離0

	for(k = 0; k < V; k++)
	{
		for(i = 0; i < V; i++)
		{
			for(j = 0; j < V; j++)
			{
				if(dist[i][k] == INF64 || dist[k][j] == INF64) continue;  // 繋がっていない辺があればスルー
				dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
			}
		}
	}

	// 負閉路を探す
	bool exist_negative_cycle = false;
	for(i = 0; i < V; i++)
	{
		if(dist[i][i] < 0) exist_negative_cycle = true;
	}

	if(exist_negative_cycle)
	{
		cout << "NEGATIVE CYCLE" << endl;		
	}
	else
	{
		for(i = 0; i < V; i++)
		{
			for(j = 0; j < V; j++)
			{
				if(dist[i][j] != INF64) cout << dist[i][j];
				else cout << "INF";
				if(j != V-1) cout << " ";
			}
			cout << endl;
		}
	}

	return 0;
}
