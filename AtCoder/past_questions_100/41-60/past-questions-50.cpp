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
 * 自力で解けず、解説を見て実装した。
 * bitDPで解く。
 * https://img.atcoder.jp/data/other/s8pc-1/s8pc-1-G.pdf
 * 
 * https://qiita.com/drken/items/7c6ff2aa4d8fce1c9361
 * の"bit DP"章を参考に、O(N^2*2^N)で巡回セールスマン問題をbitDPで実装する。
 * 
 * 基本的な実装は past-questions-49.cpp も参照のこと。
 * 
 * 最短距離に加えて「その通りの数」も求める必要があるため、
 * dp[][]には {最短距離, その通りの数} のpairを保持する。
 * また道路が閉鎖されるという条件については、
 * 距離の更新時に、距離>時間 であれば通れないようにする。
 * 
 * func((1<<N)-1, i);
 * にて得られる距離は、「最後に建物iに行った」ところまでの結果なので、
 * 建物i->店への経路をそこから追加する必要がある。
 */

const int MAX_N = 16;  // 頂点数の最大
ll dist[MAX_N][MAX_N];  // dist[i][j]:i->jへの距離
ll limit_time[MAX_N][MAX_N];  // [i][j]:i->jへの道について、閉鎖されるまでの時間

// dp[S][v]:
//   Sの各ビットをそれぞれの建物に割り当てて、
//   1になっているビットに対して好きな順に辿ったときの、
//   最小距離およびその通り数(pair<int,int>)
//   ただし最後にvに行く、という制約
pair<ll,ll> dp[1<<MAX_N][MAX_N];
int N;

// Sの各ビットをそれぞれの建物に割り当てて、
// 1になっているビットに対して好きな順に辿ったときの、
// 最小距離およびその通り数を返す(pair<int,int>)
// ただし最後にvに行く、という制約
pair<ll,ll> func(int S, int v)
{
	// メモ化再帰
	if(dp[S][v].first != -1) return dp[S][v];

	// 1になっているビットが1つ、つまり最初の建物の場合
	if(S == (1<<v))
	{
		// 開始は店(頂点0)という制約
		if(v == 0)
		{
			dp[S][v] = {0, 1};  // 距離0、かつ1通り
		}
		else
		{
			dp[S][v] = {INF64, 0};  // 店以外のスタートは無し
		}
		return dp[S][v];
	}

	// 最後にvに行くという制約より、vに行く前のSの状態を取得する
	int S_prev = S & (~(1<<v));  // vに相当するビットのみ落とす

	ll ret_dist = INF64;
	ll ret_count = 0;
	for(int i = 0; i < N; i++)
	{
		if((S_prev & (1<<i)) == 0) continue;  // 1になっていないビットはスルー
		if(dist[i][v] == INF64) continue;  // i->vへの辺が無ければスルー

		// dp[S][v]を求めたい
		// 建物vを除いた集合の最短距離(終点i) + i->Vの距離 となる

		auto e = func(S_prev, i);
		if(e.first + dist[i][v] > limit_time[i][v]) continue;  // 道路が閉鎖されるためスルー

		if(ret_dist > e.first + dist[i][v])  // 最短距離を更新する場合
		{
			ret_dist = e.first + dist[i][v];
			ret_count = e.second;  // その通りの数はリセット
		}
		else if(ret_dist == e.first + dist[i][v])  // 現状の最短距離と同じ距離の場合
		{
			ret_count += e.second;  // その通りの数は加算
		}
	}
	dp[S][v] = {ret_dist, ret_count};

	return dp[S][v];
}

int main(void)
{
	// 0-indexed
	int i, j;
	int M;
	cin >> N >> M;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			dist[i][j] = INF64;  // 辺で結ばれていない頂点間は距離をINFにする
		}
	}
	for(i = 0; i < M; i++)
	{
		ll s, t, d, tm;
		cin >> s >> t >> d >> tm;
		s--;  // 0-indexedに変換
		t--;
		dist[s][t] = dist[t][s] = d;
		limit_time[s][t] = limit_time[t][s] = tm;
	}
	// dpテーブル初期化
	for(i = 0; i < (1<<N); i++)
	{
		for(j = 0; j < N; j++)
		{
			dp[i][j] = {-1, 0};  // 未探索
		}
	}

	pair<ll,ll> answer = {INF64, 0};  // {最短距離,その通りの数}
	for(i = 0; i < N; i++)
	{
		// 各建物iを最後に行くケースについてそれぞれ探索する
		if(dist[i][0] == INF64) continue;  // 最後の建物iから店への経路が無ければスルー

		auto e = func((1<<N)-1, i);

		// 店(頂点0)が終点という制約であり、建物i->店への経路について処理
		if(e.first + dist[i][0] > limit_time[i][0]) continue;  // 道路が閉鎖されるためスルー
		e.first += dist[i][0];
		// 以下、func()の処理と同様
		if(answer.first > e.first)
		{
			answer.first = e.first;
			answer.second = e.second;
		}
		else if(answer.first == e.first)
		{
			answer.second += e.second;
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
