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

// ABC369 https://atcoder.jp/contests/abc369

// 事前ワーシャルフロイド、順列全探索＆bit全探索の重ね技問題。
// クエリのB1～Bkについて、どの順番に／どっち向きで通るかを全探索する。
// 
// ワーシャルフロイドに使うdist[]について、多重辺があるため最小値を保持する必要がある。
// クエリで橋指定があるので、入力のT自体は全て保持必要。
//
// next_permutation()について、入力Bは事前にソート済みのため、Bをそのまま使える。


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
	ll N, M; cin >> N >> M;
	vector<vector<ll>> dist(N, vector<ll>(N, INF64));  // dist[N][N]
	for(i = 0; i < N; i++) dist[i][i] = 0;
	vector<int> u(M), v(M);
	vector<ll> t(M);
	for(i = 0; i < M; i++)
	{
		cin >> u[i] >> v[i] >> t[i];
		u[i]--; v[i]--;
		// 多重辺があるので、distについては最小値だけ保持する
		// t[]自体は全て保持する必要がある
		dist[u[i]][v[i]] = dist[v[i]][u[i]] = min(dist[u[i]][v[i]], t[i]);
	}
	Warshall_Floyd(dist);

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		vector<int> b(k); for(i = 0; i < k; i++) {cin >> b[i]; b[i]--;}  // b[]は昇順ソート済み
		ll ans = INF64;
		do
		{
			for(i = 0; i < (1<<k); i++)
			{
				ll c = 0;
				int now = 0;  // 今いる頂点
				for(j = 0; j < k; j++)
				{
					int uu = u[b[j]], vv = v[b[j]];
					ll tt = t[b[j]];
					if( ((i>>j)&1) == 0)  // now->u->v
					{
						c += dist[now][uu] + tt;
						now = vv;
					}
					else  // now->v->u
					{
						c += dist[now][vv] + tt;
						now = uu;
					}
				}
				c += dist[now][N-1];
				chmin(ans, c);
			}
		} while (next_permutation(b.begin(), b.end()));
		cout << ans << endl;
	}

	return 0;
}
