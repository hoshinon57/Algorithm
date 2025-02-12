#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC022 https://atcoder.jp/contests/abc022

/*
 * 公式解説の方針で実装した内容。
 * 
 * 求めるルートは頂点0を含む閉路となる。
 * これがどういうルートになるかをもう少し考えると、
 *   頂点0と隣接する頂点を2つ選び、その頂点どうしの最短距離に
 *   頂点0との距離を加算したもの
 * となる。
 * 
 * 頂点0から出る辺を除外したグラフにてワーシャルフロイド法を適用し、
 * また頂点0と隣接する頂点について、その辺の長さを保持しておく。
 * そして頂点0と隣接する頂点について全探索。
 * 
 * 計算量はO(N^3). 41ms.
 * 最初に考えたダイクストラ法より10倍速かった。
**/

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

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<vector<ll>> dist(N, vector<ll>(N, INF64));
	vector<ll> t(N, -1);  // t[v]:頂点vが頂点0と隣であれば、その距離を設定する
	for(i = 0; i < M; i++)
	{
		int u, v, l; cin >> u >> v >> l;
		u--; v--;
		if(u > v) swap(u, v);
		if(u == 0)  // 頂点0と繋がる辺
		{
			t[v] = l;
			// distには設定しない
		}
		else
		{
			dist[u][v] = l;
			dist[v][u] = l;
		}
	}
	Warshall_Floyd(dist);

	ll ans = INF64;
	for(i = 0; i < N; i++)
	{
		if(t[i] == -1) continue;
		for(j = i+1; j < N; j++)
		{
			if(t[j] == -1) continue;
			ll tmp = dist[i][j] + t[i] + t[j];
			chmin(ans, tmp);
		}
	}
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
