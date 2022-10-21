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
 * 別解法での練習を目的として、再帰を使わず実装した。
 * 
 * DPの定義は同じく以下。
 * dp[S][v]
 *   Sのビットを各頂点に割り当てて、
 *   1になっているビットに対して好きな順でたどった時の最小距離
 *   ただし最後にvに行く、という制約
 * 
 * DP遷移の考え方も同様だが、
 * 再帰を使わないので、dp[S][v]のうちSのビットが立っている数が小さいものから順に計算していく。
 * 
 * 計算量は変わらずO(2^N*N^2) のはず。
 * DP状態数が2^N*Nで、dp[S][v]を計算するときのループがO(N).
 */

int main(void)
{
	// 0-indexed
	int i;
	int N, E;
	cin >> N >> E;
	vector<vector<int>> dist(N, vector<int>(N, INF32));  // dist[i][j]:i->jへの距離
	for(i = 0; i < E; i++)
	{
		int s, t, d;
		cin >> s >> t >> d;
		dist[s][t] = d;
	}
	// dp[S][v]
	//   Sのビットを各頂点に割り当てて、
	//   1になっているビットに対して好きな順でたどった時の最小距離
	//   ただし最後にvに行く、という制約
	vector<vector<int>> dp(1<<N, vector<int>(N, INF32));

	// dp[S][v]のSにて、bitが1になっている数が小さい方から順に計算していく
	for(int bit1_num = 1; bit1_num <= N; bit1_num++)
	{
		for(int S = 0; S < 1<<N; S++)
		{
			// Sのうち今探索したいのは、bitが立っている数がbit1_numのもののみ
			if(__builtin_popcount(S) != bit1_num) continue;

			if(bit1_num == 1)
			{
				// スタートする頂点を0(Sの最下位ビットが1)と決め打ちする
				if(S == 1) dp[S][0] = 0;
				else dp[S][0] = INF32;
				continue;  // bit1_num=2へ
			}
			// 以下、bitが立っているのが2つ以上

			// dp[S][v]を計算したい
			for(int v = 0; v < N; v++)  // 最後に到達する頂点をvとする
			{
				if((S&(1<<v)) == 0) continue;  // 頂点vに相当するbitが立っていなければスルー
				int prev_S = S&(~(1<<v));  // vに相当するbitを落とす
				// prev_Sに対して、最後の頂点を全パターン探索し、最短距離をdp[S][v]へ設定する
				for(i = 0; i < N; i++)  // i:prev_Sにおける最後の頂点
				{
					if((prev_S&(1<<i)) == 0) continue;  // 1になっていないbitはスルー
					if(dist[i][v] == INF32)continue;  // i->vへの辺が無ければスルー
					dp[S][v] = min(dp[S][v], dp[prev_S][i] + dist[i][v]);
				}
			}
		}
	}

	int answer = INF32;
	for(i = 0; i < N; i++)
	{
		// 各頂点を最後に行くケースについてそれぞれ計算し、そこから頂点0への距離を加算する
		int tmp = dp[(1<<N)-1][i] + dist[i][0];
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
