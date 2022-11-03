#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題34
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_10_A&lang=ja

/*
 * 動的計画法(もらうDP)で解く。
 * いわゆる「教科書通りの実装」であり、説明は省略する。
 */

int main(void)
{
	int n;
	cin >> n;
	vector<ll> dp(n+1);  // dp[i]:fib(i)の答

	dp[0] = dp[1] = 1;
	for(int i = 2; i <= n; i++)  // もらうDP
	{
		dp[i] = dp[i-1] + dp[i-2];
	}
	cout << dp[n] << endl;

	return 0;
}
