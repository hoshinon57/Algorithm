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

// ABC416 https://atcoder.jp/contests/abc416

/*
 * ワーシャルフロイドおよび超頂点を用いて解く。
 * 
 * クエリ1について、頂点x,yについてワーシャルフロイドの部分更新を行えばよい。
 * 部分更新の計算量はO(N^2).
 * 
 * クエリ2について、空港どうしが繋がっているため、
 * 愚直に前述の部分更新を行うとN回の更新が必要で、クエリ1回でO(N^3)となってしまう。(TLE)
 * これを空港をたばねる超頂点を用いることで対処する。
 * ワーシャルフロイドの部分更新は、空港を追加する街と超頂点の2つで行う。
 * 
 * 空港がある街D1,D2について、D1--(超頂点)--D2 の距離をTとしたいが、T/2が整数とは限らない。
 * よって距離を全体で2倍にしておき、出力時は2で割ったものを採用した。
 * (公式解説では D1->超頂点を距離T, 超頂点->D1を距離0 としていた。なるほど。)
 * 
 * 制約より多重辺がありえるので、
 * 入力時とクエリ1ではdist[]をminチェックしてから更新が必要。
 * 
 * [ACまでの思考の流れ]
 * ・N=500よりワーシャルフロイドっぽい。
 * ・クエリ1はワーシャルフロイドの部分更新でいけそう。
 * ・空港の扱いをどうするか。空港どうしがつながっているので、愚直にやるとO(N^2)とかでダメっぽい。
 *   空港をまとめる超頂点を用意しよう。
 * ・制約より多重辺がありえる。入力やクエリ1にてdist[]更新はmin処理をやっておく。
**/

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

// ワーシャルフロイド法を用いて全点対間最短経路を解く。
// dist:
//   頂点数をNとしたとき、サイズはdist[N][N]で、"INF64"で初期化していること。
//   a->bの距離をdist[a][b]として設定しておくこと。
//   (呼び出し元で、dp[i][i]=0にすることを忘れないよう注意)
void Warshall_Floyd(vector<vector<ll>> &dist, int k)
{
	int i, j;
	int N = dist.size();

//	for(k = 0; k < N; k++)
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
}

int main(void)
{
	// 頂点0～(N-1):街, N:超頂点
	ll i, j;
	ll N, M; cin >> N >> M;
	vector<vector<ll>> dist(N+1, vector<ll>(N+1, INF64));  // dist[N+1][N+1]
	for(i = 0; i < N+1; i++) dist[i][i] = 0;
	for(i = 0; i < M; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		a--; b--;
		// 距離は2倍しておく
		chmin(dist[a][b], c*2);  // 制約より、多重辺がありえる
		dist[b][a] = dist[a][b];
	}
	ll K, T; cin >> K >> T;
	for(i = 0; i < K; i++)
	{
		ll d; cin >> d;
		d--;  // 空港
		dist[d][N] = dist[N][d] = T;
	}
	for(i = 0; i < N+1; i++)
	{
		Warshall_Floyd(dist, i);
	}

	ll Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int ki; cin >> ki;
		if(ki == 1)
		{
			ll x, y, t; cin >> x >> y >> t;
			x--; y--;
			t*=2;  // 距離は2倍しておく
			chmin(dist[x][y], t);
			dist[y][x] = dist[x][y];
			Warshall_Floyd(dist, x);
			Warshall_Floyd(dist, y);
		}
		if(ki == 2)
		{
			ll x; cin >> x;
			x--;
			dist[x][N] = T;
			dist[N][x] = T;
			Warshall_Floyd(dist, x);
			Warshall_Floyd(dist, N);
		}
		if(ki == 3)
		{
			ll ans = 0;
			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
				{
					if(dist[i][j] == INF64) continue;
					ans += dist[i][j];
				}
			}
			ans /= 2;  // 距離を2倍にして計算していたため
			cout << ans << endl;
		}
	}

	return 0;
}
