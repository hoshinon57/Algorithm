#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題36
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_1_C&lang=ja

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i種類目の品物まで見たときに、 (i:1～N)
 *   重さの総和がj以下の場合の、 (j:0～W)
 *   価値の最大値
 * 
 * DP遷移は、もらうDPにて以下の最大値を取ればよい。
 * (1)i番目の品物を選ばない場合
 *   dp[i][j] = dp[i-1][j]
 * (2)i番目の品物を選ぶ場合
 *   「いくつでも選べる」という条件より、(1)を実施した後ならdp[i][j-(品物iの重さ)] を使えばよいので、
 *   dp[i][j] = dp[i][j-w[i]]+v[i]
 * 
 * 遷移式より、dp[i][j]の[i]部分を無くして1次元配列での計算も可能。
 * その場合、上記(1)の計算は不要になる。
 * 
 * 計算量はO(NW).
 */

int main(void)
{
	int i, j;
	int N, W;
	cin >> N >> W;
	vector<int> v(N+1), w(N+1);  // 1-indexed
	for(i = 1; i <= N; i++)
	{
		cin >> v[i] >> w[i];
	}

	// dp[i][j]
	//   i種類目の品物まで見たときに、 (i:1～N)
	//   重さの総和がj以下の場合の、 (j:0～W)
	//   価値の最大値
	vector<vector<ll>> dp(N+1, vector<ll>(W+1, 0));

	// もらうDP
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= W; j++)
		{
			dp[i][j] = dp[i-1][j];

			if(j-w[i] >= 0)
				dp[i][j] = max(dp[i][j], dp[i][j-w[i]]+v[i]);
		}
	}
	cout << dp[N][W] << endl;

	return 0;
}
