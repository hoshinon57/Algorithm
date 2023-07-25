#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ワーシャルフロイド法のメモ、ライブラリ
// 英語表記では Floyd–Warshall が一般的のようだが、
// 「ワーシャルフロイド」で覚えており、検索性のため warshall-floyd.cpp としている

/*
 * [関連する問題]
 * AOJ GRL_1_C https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_C
 */

// ワーシャルフロイド法を用いて全点対間最短経路を解く。
// dist:
//   頂点数をNとしたとき、サイズはdist[N][N]で、"INF64"で初期化していること。
//   a->bの距離をdist[a][b]として設定しておくこと。
//   (呼び出し元で、dp[i][i]=0にすることを忘れないよう注意)
// 負閉路があればtrueを返す。
bool Warshall_Floyd(vector<vector<ll>> &dist)
{
	int i, j, k;
	int N = dist.size();

	for(k = 0; k < N; k++)
	{
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				if(dist[i][k] != INF64 && dist[k][j] != INF64)  // i-k, k-j間がどちらも通れる場合
				{
					dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
				}
			}
		}
	}

	// dist[v][v] < 0 が存在する場合、負閉路がある
	bool exist_negative_cycle = false;
	for(i = 0; i < N; i++)
	{
		if(dist[i][i] < 0) exist_negative_cycle = true;
	}

	return exist_negative_cycle;
}

void solve(void)
{
	// 以下はAOJのGRL_1_Cを解く内容
	// https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_C

	int i, j;
	int V, E; cin >> V >> E;
	vector<vector<ll>> dist(V, vector<ll>(V, INF64));
	for(i = 0; i < V; i++) dist[i][i] = 0;
	for(i = 0; i < E; i++)
	{
		ll s, t, d; cin >> s >> t >> d;
		dist[s][t] = d;
	}
	bool negative_cycle = Warshall_Floyd(dist);

	if(negative_cycle)
	{
		cout << "NEGATIVE CYCLE" << endl;
		return;
	}
	for(i = 0; i < V; i++)
	{
		for(j = 0; j < V; j++)
		{
			if(dist[i][j] == INF64)
				cout << "INF";
			else
				cout << dist[i][j];
			if(j != V-1) cout << " ";
		}
		cout << endl;
	}
	return;
}

int main(void)
{
	solve();

	return 0;
}
