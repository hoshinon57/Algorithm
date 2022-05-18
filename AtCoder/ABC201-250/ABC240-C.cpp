#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;

// ABC240 https://atcoder.jp/contests/abc240

/*
 * 動的計画法で解く。
 * dp[i][j]
 * i回目までジャンプしたときに、   i:1～N
 * 座標jにいることが可能かどうか   j:0～10000
 * 可能ならtrue
 */
int main(void)
{
	// 1-indexed
	int i, j;
	int N, X;
	cin >> N >> X;
	vector<int> a(N), b(N);
	for(i = 1; i <= N; i++)
	{
		cin >> a[i] >> b[i];
	}

	// dp[i][j]
	// i回目までジャンプしたときに、   i:1～N
	// 座標jにいることが可能かどうか   j:0～10000
	// 可能ならtrue
	vector<vector<bool>> dp(N+1, vector<bool>(10001, false));
	dp[0][0] = true;

	// 配るDP
	for(i = 0; i < N; i++)
	{
		for(j = 0; j <= 10000; j++)
		{
			if(dp[i][j])  // ここにいることが可能
			{
				if(j+a[i+1] <= 10000)	dp[i+1][j+a[i+1]] = true;
				if(j+b[i+1] <= 10000)	dp[i+1][j+b[i+1]] = true;
			}
		}
	}

	if(dp[N][X])
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
