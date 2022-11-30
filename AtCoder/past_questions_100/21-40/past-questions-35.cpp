#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題35
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_1_B&lang=ja

/*
 * 動的計画法(もらうDP)で解く。
 * いわゆる「教科書通りの実装」であり、説明は省略する。
 * 
 * 計算量はO(NW).
 */

int main(void)
{
	int i, j;
	int N, W;
	cin >> N >> W;
	vector<int> v(N+1), w(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> v[i] >> w[i];
	}
	
	// dp[i][j]:
	//   i個目の品物まで見たときに、  (i=0～N)
	//   重さがjを超えないようにしたときの、  (j=0～W)
	//   価値の最大値
	vector<vector<int>> dp(N+1, vector<int>(W+1));
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= W; j++)
		{
			dp[i][j] = dp[i-1][j];  // i個目の品物を選ばない場合
			if(j-w[i] >= 0)
				dp[i][j] = max(dp[i][j], dp[i-1][j-w[i]]+v[i]);  // i個目の品物を選ぶ場合
		}
	}
	cout << dp[N][W] << endl;

	return 0;
}
