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

// ABC075 https://atcoder.jp/contests/abc075

// ワーシャルフロイド法で解いてみた版。
// 消す辺iについて全探索。
// ワーシャルフロイド法を適用した後、頂点0からの距離でINFのものがあれば、その辺は橋。

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
	vector<int> a(M), b(M); for(i = 0; i < M; i++) {cin >> a[i] >> b[i]; a[i]--; b[i]--;}

	int ans = 0;
	for(i = 0; i < M; i++)  // 消す辺i
	{
		vector<vector<ll>> dist(N, vector<ll>(N, INF64));
		for(j = 0; j < N; j++) dist[j][j] = 0;
		for(j = 0; j < M; j++)
		{
			if(j == i) continue;
			dist[a[j]][b[j]] = dist[b[j]][a[j]] = 1;
		}
		Warshall_Floyd(dist);

		// 頂点0からの距離でINFのものがあれば、橋
		if(*max_element(dist[0].begin(), dist[0].end()) == INF64) ans++;
	}
	cout << ans << endl;

	return 0;
}
