#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC151 https://atcoder.jp/contests/abc151

/*
 * ワーシャルフロイド法でも解いてみる。
 * 
 * H*Wの頂点を用意する。 -> dist[H*W]
 * dist[]の初期化として、上下左右のマスとの距離を1に設定する。
 * これはマス(i,j)に対してdist[]のインデックスを i*W+j で計算すればよい。
 * 
 * あとは用意したワーシャルフロイドのライブラリにて全頂点間の距離を計算し、その最大値が答となる。
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

int main(void)
{
	// 0-indexed
	int i, j;
	int H, W; cin >> H >> W;
	vector<string> mp(H);
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
	}
	const int V_NUM = H * W;
	vector<vector<ll>> dist(V_NUM, vector<ll>(V_NUM, INF64));  // dist[V_NUM][V_NUM]
	for(i = 0; i < V_NUM; i++) dist[i][i] = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] == '#') continue;
			// 上下左右のマスとの距離を1に設定する
			int idx = i*W + j;
			if(i > 0   && mp[i-1][j] != '#') dist[idx][idx-W] = dist[idx-W][idx] = 1;  // 上
			if(i < H-1 && mp[i+1][j] != '#') dist[idx][idx+W] = dist[idx+W][idx] = 1;  // 下
			if(j > 0   && mp[i][j-1] != '#') dist[idx][idx-1] = dist[idx-1][idx] = 1;  // 左
			if(j < W-1 && mp[i][j+1] != '#') dist[idx][idx+1] = dist[idx+1][idx] = 1;  // 右
		}
	}
	Warshall_Floyd(dist);

	// 全要素のうち最大値が答
	ll ans = 0;
	for(i = 0; i < V_NUM; i++)
	{
		for(j = 0; j < V_NUM; j++)
		{
			if(dist[i][j] == INF64) continue;
			ans = max(ans, dist[i][j]);
		}
	}
	cout << ans << endl;

	return 0;
}
