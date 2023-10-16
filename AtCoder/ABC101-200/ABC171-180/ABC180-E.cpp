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

// ABC180 https://atcoder.jp/contests/abc180

/*
 * 通常の巡回セールスマン問題を解けばよい。
 * 公式解説の「都市間の移動のコストについて三角不等式が成り立つので」という記載については、以下URLが参考になった。
 *   https://drken1215.hatenablog.com/entry/2020/10/21/172509
 * ※都市A->Bに行く場合、他の都市を経由せず直接Bに行けばよいという考え方。
 *   仮に距離が
 *     A->B:3
 *     A->C:1
 *     C->B:1
 *   ならばCを経由した方が早いので、まずワーシャルフロイド法にて各頂点間の距離を求める必要がある。
 * 
 * あとはbitDPライブラリをペタリと貼って終了。
 */

const int MAX_N = 17;  // 頂点数の最大
int dist[MAX_N][MAX_N];  // dist[i][j]:頂点i->jへの距離
// dp[S][v]:
//   Sの各ビットをそれぞれの都市に割り当てて、
//   1になっているビットに対して好きな順に辿ったときの、最小距離
//   ただし最後にvに行く、という制約
//   未探索であれば-1
int dp[1<<MAX_N][MAX_N];

// Sの各ビットをそれぞれの都市に割り当てて、
// 1になっているビットに対して好きな順に辿ったときの、最小距離を返す
// ただしスタート地点を0とし、最後にvに行く、という制約
// N:頂点数
// 呼び出し元での事前準備：
//   dist[][]に頂点間の距離を設定する
//   dp[][]を未探索の-1にする
int bitDP(int S, int v, int N)
{
	// メモ化再帰で解く
	if(dp[S][v] != -1) return dp[S][v];  // メモ化済み

	if(S == 0)  // ベースケース
	{
		// 頂点0をスタートとするので、dp[0][0]は0, dp[0][*]はありえないためINF.
		if(v == 0) dp[S][v] = 0;
		else dp[S][v] = INF32;

		return dp[S][v];
	}

	if(((S >> v) & 1) == 0) {dp[S][v] = INF32; return dp[S][v];}  // Sに頂点vが含まれていない
	int S_prev = S & (~(1<<v));  // vに相当するビットを落とす

	int &d = dp[S][v];
	d = INF32;
	for(int u = 0; u < N; u++)
	{
		// S_prev内の頂点uを決めて、u->vへの遷移を考える
		if(dist[u][v] == INF32) continue;  // u->vへの辺が無ければスルー
		
		// dp[S][v] = dp[S-{v}][u] + dist[u][v]
		d = min(d, bitDP(S_prev, u, N) + dist[u][v]);
	}
	return d;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> x(N), y(N), z(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i] >> z[i];
	}
	// dist計算
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)  // dist[i][j]を求める !=dist[j][i]
		{
			dist[i][j] = abs(x[j]-x[i]) + abs(y[j]-y[i]) + max(0LL, z[j]-z[i]);
		}
	}
	// dpテーブル初期化
	for(i = 0; i < 1<<N; i++)
	{
		for(j = 0; j < N; j++) dp[i][j] = -1;
	}

	int ans = bitDP((1<<N)-1, 0, N);
	cout << ans << endl;

	return 0;
}
