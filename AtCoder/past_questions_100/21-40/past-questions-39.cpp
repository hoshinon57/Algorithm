#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題39
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2011yo/tasks/joi2011yo_d

/*
 * 動的計画法で解く。
 * 以下、1-indexedで記載する。
 * 
 * dp[i][j]を以下のように定義する。
 *   i個目の数字までを見たときに、  (i:1～N-1)
 *   式の結果がjになるような  (j:0～20)
 *   式の数
 * 
 * DP遷移は、i個目の数値をnum[i]とすると、
 *   dp[i+1][j+num[i+1]] += dp[i][j];  // 加算するケース
 *   dp[i+1][j-num[i+1]] += dp[i][j];  // 減算するケース
 * となる。
 * ただし、最初の数値は+のみであるため、初回は減算しない。
 * ⇒この除外が無いと、最初の数値が0のときにNGになってしまう。
 * 
 * 計算量は、JOI君が計算できる上限をWとすると(問題文では20)、O(NW)となる。
 */
int main(void)
{
	// 1-indexed
	int i, j;
	int N;
	cin >> N;
	vector<int> num(N);  // 1-indexedでN-1個のため、添字は1～N-1の間
	int answer;
	for(i = 1; i <= N-1; i++) cin >> num[i];
	cin >> answer;

	// dp[i][j]
	//   i個目の数字までを見たときに、  (i:1～N-1)
	//   式の結果がjになるような  (j:0～20)
	//   式の数
	vector<vector<ll>> dp(N, vector<ll>(20+1, 0));
	dp[0][0] = 1;  // 初期設定 計算する前は結果が0のみ

	// 配るDP
	for(i = 0; i < N-1; i++)
	{
		for(j = 0; j <= 20; j++)
		{
			// 加算するケース
			if(j+num[i+1] <= 20)
				dp[i+1][j+num[i+1]] += dp[i][j];

			// 減算するケース
			// 最初の数値は+のみであり、i==0は除外する (除外しないと、最初の数値が0のときにNGになってしまう)
			if(j-num[i+1] >= 0 && i != 0)
				dp[i+1][j-num[i+1]] += dp[i][j];
		}
	}

	// 最後の数まで見たときに、式の結果がanswerになる数
	cout << dp[N-1][answer] << endl;

	return 0;
}
