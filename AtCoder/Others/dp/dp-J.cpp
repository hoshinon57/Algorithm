#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 期待値DPで解く。
 * 
 * まず、
 * 「皿1は寿司0個、皿2は寿司2個、皿3は寿司2個」を、  (問題文の表現)
 * 「寿司0個の皿は1つ、寿司2個の皿は2つ」に読み替えてよい。
 * 
 * dp[i][j][k]を以下のように定める。
 *   寿司が1個置かれている皿の数をi,  i:0～N
 *   寿司が2個置かれている皿の数をj,  i:0～N
 *   寿司が3個置かれている皿の数をk,  i:0～N
 *   としたときの、すべての寿司がなくなるまでの回数の期待値
 * 
 * 遷移は以下のようになる。
 *   dp[i][j][k] 
 *   = (dp[i-1][j  ][k  ]+1)*(i/N)   // 1個置かれている皿から食べる場合  選ぶ確率はi/N  +1は操作回数の加算
 *   + (dp[i+1][j-1][k  ]+1)*(j/N)   // 2個
 *   + (dp[i  ][j+1][k-1]+1)*(k/N)   // 3個
 *   + (dp[i  ][j  ][k  ]+1)*((N-i-j-k)/N)  // 0個置かれている皿を選んだ場合  選ぶ確率は(N-i-j-k)/N
 * dp[i][j][k]が両辺にあるので、整理すると以下になる。
 *   dp[i][j][k]*(i+j+k)
 *   = N
 *   + dp[i-1][j  ][k  ]*i
 *   + dp[i+1][j-1][k  ]*j
 *   + dp[i  ][j+1][k-1]*k
 * あとはこれを解いていけばよい。
 * 
 * (1)再帰で解く場合
 * メモ化再帰にしないとTLEとなる。
 * 
 * (2)再帰を使わない場合
 * 3重ループで解く。
 * 遷移式より、i,j,kの順番かつ小さい方からループする必要がある。
 */

// double dp_memo[301][301][301] = {0.0};
double dp_memo[302][302][302] = {0.0};  // 再帰で解くなら[301]で良い 再帰を使わない3重ループの場合、[302]にする必要がある
int dp_memo_flag[301][301][301] = {0};  // 1ならメモ化済
int sushi_num[3+1] = {0};  // sushi_num[i]:i個の寿司が置かれている皿の数  i:1～3
int N;

double func(int i, int j, int k)
{
	// ベースケース
	if(i == 0 && j == 0 && k == 0) return 0.0;

	// メモを返す
	if(dp_memo_flag[i][j][k] == 1) return dp_memo[i][j][k];

	double tmp = 0.0;
	tmp += N;
	if(i > 0) tmp += func(i-1, j,   k  )*i;
	if(j > 0) tmp += func(i+1, j-1, k  )*j;
	if(k > 0) tmp += func(i,   j+1, k-1)*k;
	tmp = tmp / (double)(i+j+k);
	// メモ化
	dp_memo[i][j][k] = tmp;
	dp_memo_flag[i][j][k] = 1;

	return tmp;
}

int main(void)
{
	// 1-indexed
	cin >> N;
	for(int i = 1; i <= N; i++)
	{
		int a;
		cin >> a;
		sushi_num[a]++;
	}
	dp_memo[0][0][0] = 0.0;  // DP初期値

	cout << std::fixed << std::setprecision(10);
#if 1
	cout << func(sushi_num[1], sushi_num[2], sushi_num[3]) << endl;	
#else  // 再帰を使わない場合はelse側を有効にする
	for(int k = 0; k <= N; k++)
	{
		for(int j = 0; j <= N; j++)
		{
			// <=N ではなく <=N+1 にした方が良い(i=N, j>0のときに未計算のdp[]を使うことになるため)。
			// ただし問題の制約により i+j<=N になるので、正しい結果は得られる。
			for(int i = 0; i <= N; i++)
			{
				if(i == 0 && j == 0 && k == 0) continue;
				double tmp = 0.0;
				tmp += N;
				if(i > 0) tmp += dp_memo[i-1][j  ][k  ]*i;
				if(j > 0) tmp += dp_memo[i+1][j-1][k  ]*j;
				if(k > 0) tmp += dp_memo[i  ][j+1][k-1]*k;
				tmp = tmp / (double)(i+j+k);
				dp_memo[i][j][k] = tmp;
			}
		}
	}
	cout << dp_memo[sushi_num[1]][sushi_num[2]][sushi_num[3]] << endl;
#endif

	return 0;
}
