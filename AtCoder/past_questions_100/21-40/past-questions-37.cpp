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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題37
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_1_A&lang=ja

/*
 * 動的計画法(もらうDP)で解く。
 * いわゆる「教科書通りの実装」であり、説明は省略する。
 * 
 * 計算量はO(mn).
 */
int main(void)
{
	int i, j;
	int n, m; cin >> n >> m;
	vector<int> c(m+1); for(i = 1; i <= m; i++) {cin >> c[i];}

	// dp[i][j]:
	//   i種類目のコインまで見たときに、  (i:1～m)
	//   j円を支払うときの、  (j:0～n)
	//   コインの最小枚数
	vector<vector<int>> dp(m+1, vector<int>(n+1, INF32));  // dp[M+1][N+1]
	dp[0][0] = 0;

	for(i = 1; i <= m; i++)
	{
		for(j = 0; j <= n; j++)
		{
			// まずは前回の値をコピー
			dp[i][j] = dp[i-1][j];

			// 同じ硬貨は何枚でも使えるので、
			// dp[i][ j-今見ているコインの額面 ]に1を加えたものと比較する
			if(j-c[i] >= 0)
			{
				dp[i][j] = min(dp[i][j], dp[i][j-c[i]]+1);
			}
		}
	}
	cout << dp[m][n] << endl;
	
	return 0;
}
