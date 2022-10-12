#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

/*
 * [関連する問題]
 * https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_2_A&lang=ja
 */

/*
 * 以下、bitDPの典型問題として、巡回セールスマン問題をO(N*2^N)で解く。
 * 問題文：https://qiita.com/drken/items/7c6ff2aa4d8fce1c9361 の"bit DP"
 * 
 * dp[S][v]を以下のように定める。
 *   Sの各ビットをそれぞれの都市に割り当てて、
 *   1になっているビットに対して好きな順に辿ったときの、最小距離
 *   ただし最後にvに行く、という制約
 *   未探索であれば-1
 * 
 * 例えばS=b'1101, v=0であれば、
 *   頂点0,2,3を好きな順に辿ったときの、最小距離
 *   ただし最後に行くのは頂点0
 * となる。
 * 
 * dpの遷移式は以下となる。
 *   dp[S][v] = min(dp[S-{v}][i] + dist[i][v])
 *   ※ただしiは、S-{v}に含まれる頂点iそれぞれについて計算する
 * 頂点vを除いた集合の移動距離(終点i) + i->vの距離 という考え方。
 * 
 * あとは上記を実装した関数func()を用意し、各頂点vに対して
 *   func((全頂点のbitを1にした値), v)
 * の最小値が答となる。
 */

const int MAX_N = 20;
int dist[MAX_N][MAX_N];  // dist[i][j]:i->jへの距離
int dp[(1<<MAX_N)][MAX_N];

int N;

// Sの各ビットをそれぞれの都市に割り当てて、
// 1になっているビットに対して好きな順に辿ったときの、最小距離
// ただし最後にvに行く、という制約
int func(int S, int v)
{
	// メモ化再帰
	if(dp[S][v] != -1) return dp[S][v];

	// Sにて1になっているビットが1つ、つまり最初の都市の場合
	if(S == (1<<v))
	{
		dp[S][v] = 0;  // まだ移動していないので、距離は0
		return dp[S][v];
	}

	// 最後にvに行くという制約より、vに行く前のSの状態を取得する
	int S_prev = S & (~(1<<v));  // vに相当するビットのみ落とす

	int ret = INF32;
	for(int i = 0; i < N; i++)
	{
		if((S_prev & (1<<i)) == 0) continue;  // 1になっていないビットはスルー

		// dp[S][v]を求めたい
		// 都市vを除いた集合の移動距離(終点i) + i->vの距離 となる

		// dp[S][v] = dp[S-{v}][i] + dist[i][v]
		ret = min(ret, func(S_prev, i) + dist[i][v]);
	}
	dp[S][v] = ret;

	return dp[S][v];
}

int main(void)
{
	int i, j;
	cin >> N;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			cin >> dist[i][j];
		}
	}

	// dpテーブル初期化
	for(i = 0; i < (1<<N); i++)
	{
		for(j = 0; j < N; j++)
		{
			dp[i][j] = -1;  // 未探索
		}
	}

	int answer = INF32;
	for(i = 0; i < N; i++)
	{
		answer = min(answer, func((1<<N)-1, i));
	}
	cout << answer;

	return 0;
}
