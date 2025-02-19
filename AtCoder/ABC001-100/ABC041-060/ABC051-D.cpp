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

// ABC051 https://atcoder.jp/contests/abc051

// ワーシャルフロイドをして、a-b間の結果がcと異なれば、その辺cは使われない。

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
	int N, M; cin >> N >> M;
	vector<vector<ll>> dist(N, vector<ll>(N, INF64));
	for(i = 0; i < N; i++) dist[i][i] = 0;
	vector<int> a(M), b(M), c(M);
	for(i = 0; i < M; i++) {
		cin >> a[i] >> b[i] >> c[i];
		a[i]--; b[i]--;
		dist[a[i]][b[i]] = dist[b[i]][a[i]] = c[i];
	}
	Warshall_Floyd(dist);

	int ans = 0;
	for(i = 0; i < M; i++)
	{
		if(dist[a[i]][b[i]] != c[i]) ans++;
	}
	cout << ans << endl;

	return 0;
}
