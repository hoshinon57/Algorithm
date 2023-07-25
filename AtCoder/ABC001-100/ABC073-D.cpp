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

// ABC073 https://atcoder.jp/contests/abc073

/*
 * ワーシャルフロイドと順列全探索で解く。
 * 
 * まずは全町間の距離をワーシャルフロイド法で求める。
 * r1,r2,...,rRへの訪れ方は、next_permutation()を用いて全パターン探索する。
 * 各訪れ方ごとに移動距離を計算し、その最小値が答となる。
 * 
 * 計算量はワーシャルフロイド部分がO(N^3), 順列全探索の部分がO(R!)であり、
 * 全体でO(N^3 + R!)となる。
 * N,Rどちらも小さい値のため、十分に間に合う。
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
	int i;
	int N, M, R; cin >> N >> M >> R;
	vector<int> r(R); for(i = 0; i < R; i++) {cin >> r[i]; r[i]--;}
	sort(r.begin(), r.end());  // 念のためソートしておく

	vector<vector<ll>> dist(N, vector<ll>(N, INF64));
	for(i = 0; i < M; i++)
	{
		int a, b, c; cin >> a >> b >> c;
		a--; b--;
		dist[a][b] = dist[b][a] = c;
	}
	for(i = 0; i < N; i++) dist[i][i] = 0;
	Warshall_Floyd(dist);

	ll ans = INF64;
	do
	{
		ll sum = 0;
		for(i = 0; i < R-1; i++)
		{
			sum += dist[r[i]][r[i+1]];
		}
		ans = min(ans, sum);
	} while (next_permutation(r.begin(), r.end()));
	cout << ans << endl;

	return 0;
}
