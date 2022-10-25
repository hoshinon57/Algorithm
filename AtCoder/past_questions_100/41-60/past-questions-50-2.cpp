#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題50
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/s8pc-1/tasks/s8pc_1_g

/*
 * bitDPで解く。
 * 別解法での練習を目的として、再帰を使わず実装した。
 * 
 * DPの定義は同じく以下。
 * dp[S][v]:
 *   Sの各ビットをそれぞれの建物に割り当てて、
 *   1になっているビットに対して好きな順に辿ったときの、
 *   最小距離およびその通り数(pair<int,int>)
 *   ただし最後にvに行く、という制約
 * 
 * DP遷移の考え方も同じだが、
 * 再帰を使わないため、Sにてビットが立っている数が少ないものから順に計算していく。
 * 
 * 計算量は変わらずO(2^N * N^2) のはず。
 * DP状態数が2^N*Nで、dp[S][v]を計算するときのループがO(N).
 */

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	vector<vector<ll>> dist(N, vector<ll>(N, INF64));  // dist[i][j]:i->jへの距離
	vector<vector<ll>> limit_time(N, vector<ll>(N, INF64));  // [i][j]:i->jへの道について、閉鎖されるまでの時間
	for(i = 0; i < M; i++)
	{
		ll s, t, d, tm;
		cin >> s >> t >> d >> tm;
		s--;  // 0-indexedに変換
		t--;
		dist[s][t] = dist[t][s] = d;
		limit_time[s][t] = limit_time[t][s] = tm;
	}
	// dp[S][v]:
	//   Sの各ビットをそれぞれの建物に割り当てて、
	//   1になっているビットに対して好きな順に辿ったときの、
	//   最小距離およびその通り数(pair<int,int>)
	//   ただし最後にvに行く、という制約
	using pll = pair<ll,ll>;
	vector<vector<pll>> dp(1<<N, vector<pll>(N, {INF64, 0}));

	// dp[S][v]のSにて、bitが1になっている数が少ない方から順に計算していく
	for(int bit1_num = 1; bit1_num <= N; bit1_num++)
	{
		for(int S = 0; S < 1<<N; S++)
		{
			// Sのうち今探索したいのは、bitが立っている数がbit1_numのもののみ
			if(__builtin_popcount(S) != bit1_num) continue;

			if(bit1_num == 1)
			{
				// スタートは店(頂点0)という制約
				if(S == 1) dp[S][0] = {0, 1};  // 距離0、かつ1通り
				// その他はINF
				continue;				
			}
			// 以下、bitが立っている数が2つ以上

			// dp[S][v]を計算する
			for(int v = 0; v < N; v++)
			{
				if((S & (1<<v)) == 0) continue;  // vに相当するビットが立っていなければスルー
				int prev_S = S & (~(1<<v));  //vに相当するビットを落とす

				// dp[S][v] = dp[S-{v}][i] + dist[i][v]
				ll ret_dist = INF64;
				ll ret_count = 0;
				for(i = 0; i < N; i++)
				{
					if((prev_S & (1<<i)) == 0) continue;
					if(dp[prev_S][i].first + dist[i][v] > limit_time[i][v]) continue;  // 道路が閉鎖される

					if(ret_dist > dp[prev_S][i].first + dist[i][v])  // 最短距離を更新する場合
					{
						ret_dist = dp[prev_S][i].first + dist[i][v];
						ret_count = dp[prev_S][i].second;  // 通りの数はリセット
					}
					else if(ret_dist == dp[prev_S][i].first + dist[i][v])  // 現状の最短距離と同じ距離の場合
					{
						ret_count += dp[prev_S][i].second;  // 通りの数は加算
					}
				}
				dp[S][v] = {ret_dist, ret_count};
			}
		}
	}

	pll answer = {INF64, 0};  // {最短距離,その通りの数}
	for(i = 0; i < N; i++)
	{
		// 各建物を最後に行くケースについてそれぞれ探索する
		if(dist[i][0] == INF64) continue;  // 最後の建物iから店への道が無い

		ll dp_first  = dp[(1<<N)-1][i].first;
		ll dp_second = dp[(1<<N)-1][i].second;
		// 以下、前述のdp[S][v]を求めるときの処理と同じ内容
		if(dp_first + dist[i][0] > limit_time[i][0]) continue;  // 道路が閉鎖される
		if(answer.first > dp_first + dist[i][0])  // 最短距離を更新する場合
		{
			answer.first = dp_first + dist[i][0];
			answer.second = dp_second;  // その通りの数はリセット
		}
		else if(answer.first == dp_first + dist[i][0])  // 現状の最短距離と同じ距離の場合
		{
			answer.second += dp_second;  // その通りの数は加算
		}
	}

	if(answer.first != INF64)
	{
		cout << answer.first << " " << answer.second << endl;
	}
	else
	{
		cout << "IMPOSSIBLE" << endl;
	}

	return 0;
}
