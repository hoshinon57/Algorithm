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
 * 
 * past-questions-39.cppと違い、もらうDPで解いてみた。
 * 配るDPでの「最初の数値は+のみ」という判定が不要になるので、こちらの方が楽かも。
 */
int main(void)
{
	// 0-indexed
	int i, j;
	int N;
	cin >> N;
	vector<int> num(N-1);
	int answer;
	for(i = 0; i < N-1; i++) cin >> num[i];
	cin >> answer;

	// dp[i][j]
	//   i番目の数字までを見たときに、  (i:0～N-2)
	//   式の結果がjになるような  (j:0～20)
	//   式の数
	vector<vector<ll>> dp(N-1, vector<ll>(20+1, 0));
	dp[0][num[0]] = 1;  // 初期設定 1番目の数値は+のみ

	// もらうDP
	for(i = 1; i < N-1; i++)
	{
		for(j = 0; j <= 20; j++)
		{
			if(j-num[i] >= 0)
				dp[i][j] += dp[i-1][j-num[i]];  // num[i]を加算として使うケース
			if(j+num[i] <= 20)
				dp[i][j] += dp[i-1][j+num[i]];  // num[i]を減算として使うケース
		}
	}

	// 最後の数まで見たときに、式の結果がanswerになる数
	cout << dp[N-2][answer] << endl;

	return 0;
}
