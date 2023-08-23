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

// ABC243 https://atcoder.jp/contests/abc243

/*
 * ワーシャルフロイド法で解く。
 * 
 * 削除できる辺は、
 *   頂点i,jを繋ぐ辺の長さcに対して、
 *   i,j以外の辺を使って（回り道して）到達する長さc'で、
 *   c >= c' となるような経路が存在する
 * とき、頂点i,jを繋ぐ辺を削除できる。
 * 
 * よって以下のように実装した。
 *   ・一般的なワーシャルフロイド法をコードを用意する。（自分はライブラリ化している）
 *   ・dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]); の部分を書き換えて、
 *     i,jが直接つながっているときに、dist[i][j]以下となるような迂回路が存在すれば、
 *     i,jを繋ぐ辺は削除できると判断し、ansを+1する。
 *   ・同じ辺を2回削除しないようなケアは必要。
 *   ・dist[i][j]とdist[j][i]で2回カウントされるため、/2して出力。
 * 
 * 各所の解説では「ワーシャルフロイド法にて最短距離を求めた"後に"」と書かれているが、
 * 自分の実装ではワーシャルフロイド法をやりながら求めた形になる。
 */

vector<vector<bool>> connect;  // connect[i][j]:頂点i,jが辺で直接繋がっていればtrue
int ans;

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
				// 本問題に限っては、dist[x][x]を使うようなケースは省く
				// dist[i][j] >= dist[i][i]+dist[i][j] となるようなケースで誤判定してしまうため
				if(i == k || j == k) continue;
				
				if(dist[i][k] != INF64 && dist[k][j] != INF64)  // i-k, k-j間がどちらも通れる場合
				{
					if(dist[i][j] >= dist[i][k]+dist[k][j])
					{
						if(connect[i][j])  // i,jが直接つながっている場合
						{
							ans++;
							connect[i][j] = false;  // 2回チェックしないよう、falseにしておく
						}
						dist[i][j] = dist[i][k]+dist[k][j];
					}
//					dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
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
	connect.resize(N, vector<bool>(N, false));
	for(i = 0; i < N; i++) dist[i][i] = 0;
	for(i = 0; i < M; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		a--; b--;
		dist[a][b] = dist[b][a] = c;
		connect[a][b] = connect[b][a] = true;
	}

	Warshall_Floyd(dist);
	cout << ans/2 << endl;  // i->jとj->iでダブルカウントされるため

	return 0;
}
