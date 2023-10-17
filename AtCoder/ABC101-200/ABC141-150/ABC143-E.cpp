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

// ABC143 https://atcoder.jp/contests/abc143

/*
 * 自力で解けず、解説を見た。
 * ワーシャルフロイド法で解く。
 * 
 * まずは入力A,B,Cを元にワーシャルフロイドを実施する。
 * これで得られたdist(距離データ)について、
 *   L以下なら1
 *   Lより大きいならINF
 * に置き換える。
 * dist[a][b]について、a->bへ補給無しで行けるなら1, 補給が必要ならINFというイメージ。
 * 
 * このdistを使って再度ワーシャルフロイドを実施すると、
 * dist[a][b]にはa->bへ移動する際の補給回数が設定されている。
 * 
 * 1回目のワーシャルフロイドにて、各頂点から補給無しで行ける頂点を列挙し、
 * 2回目でそれらを統合するイメージ。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・N<=300という制約からワーシャルフロイドっぽい。
 *   Q<=N(N-1)より、事前に全頂点間の距離を求める必要があるので、なおさら。
 * ・頂点間の距離に「残り燃料」というのを加味すると、うまいワーシャルフロイドが見つからない。
 * ・最適行動を考えると、「行けるだけ行って、燃料が尽きたら補給」になりそう。
 *   「補給無し」であれば、各頂点から行ける頂点は簡単に求められるので、
 *   ならその情報を組み合わせる形で求められる。
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
	ll i, j;
	ll N, M, L; cin >> N >> M >> L;
	vector<vector<ll>> dist(N, vector<ll>(N, INF64));  // dist[N][N]
	for(i = 0; i < M; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		a--; b--;
		dist[a][b] = dist[b][a] = c;
	}
	for(i = 0; i < N; i++) dist[i][i] = 0;

	Warshall_Floyd(dist);  // 1回目

	// 補給無しで行ける場合は1, 行けない場合はINF64のコストの辺を張りなおす
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(dist[i][j] <= L) dist[i][j] = 1;
			else dist[i][j] = INF64;
		}
	}

	Warshall_Floyd(dist);  // 2回目

	ll Q; cin >> Q;
	while(Q > 0)
	{
		ll s, t; cin >> s >> t;
		s--; t--;
		ll ans = -1;
		if(dist[s][t] != INF64) ans = dist[s][t]-1;  // -1は、出発時点では燃料が満タンのため
		cout << ans << endl;
		Q--;
	}

	return 0;
}
