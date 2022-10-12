#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題49
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_2_A&lang=ja

/*
 * bitDPで解く。
 * 
 * https://qiita.com/drken/items/7c6ff2aa4d8fce1c9361
 * の"bit DP"章を参考に、O(N*2^N)で巡回セールスマン問題をbitDPで実装する。
 * このときネックになるのが、問題文の
 * ＞ある頂点から出発し、出発点へ戻る閉路である。
 * という条件。
 * これは 頂点0～N-1 の頂点をそれぞれ1回ずつ通る閉路になる。
 * つまり、「頂点0をスタートとして、頂点0に戻る」と決め打ちして良い。
 * 参考：
 *   https://qiita.com/Ll_e_ki/items/fa475f5bb224ada9be97
 *   https://tutuz.hateblo.jp/entry/2018/07/17/021422
 * 
 * func((1<<N)-1, i);
 * にて得られる距離は、「最後に頂点iに行った」ところまでの距離なので、
 * 頂点i->0への距離である dist[i][0] を加算したものが巡回距離となる。
 * この最小値を求めればよい。
 */

const int MAX_N = 15;  // 頂点数の最大
int dist[MAX_N][MAX_N];  // dist[i][j]:i->jへの距離
// dp[S][v]:
//   Sの各ビットをそれぞれの都市に割り当てて、
//   1になっているビットに対して好きな順に辿ったときの、最小距離
//   ただし最後にvに行く、という制約
//   未探索であれば-1
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
		// スタートする頂点を0と決め打ちする
		if(v == 0)
		{
			dp[S][v] = 0;  // まだ移動していないので、距離は0
		}
		else
		{
			dp[S][v] = INF32;  // 頂点0以外のスタートは無し
		}
		return dp[S][v];
	}

	// 最後にvに行くという制約より、vに行く前のSの状態を取得する
	int S_prev = S & (~(1<<v));  // vに相当するビットのみ落とす

	int ret = INF32;
	for(int i = 0; i < N; i++)
	{
		if((S_prev & (1<<i)) == 0) continue;  // 1になっていないビットはスルー
		if(dist[i][v] == INF32) continue;  // i->vへの辺が無ければスルー

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
	// 0-indexed
	int i, j;
	int E;
	cin >> N >> E;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			dist[i][j] = INF32;  // 辺で結ばれていない頂点間は距離をINFにする
		}
	}
	for(i = 0; i < E; i++)
	{
		int s, t, d;
		cin >> s >> t >> d;
		dist[s][t] = d;
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
		// 各頂点を最後に行くケースについてそれぞれ探索
		int tmp = func((1<<N)-1, i);
		tmp += dist[i][0];  // 頂点0が始点かつ終点という決め打ちであり、i->0 への距離を加算
		
		answer = min(answer, tmp);
	}
	if(answer != INF32)
	{
		cout << answer << endl;
	}
	else
	{
		cout << -1 << endl;
	}

	return 0;
}
