#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// https://atcoder.jp/contests/abc242

const ll MOD = 998244353;
int main(void)
{
	int i, j;
	int N;
	cin >> N;
	// dp[1～N][1～9] -> dp[N+1][10] -> dp計算を簡単にするためにdp[N+1][11]にしておく
	// dp[i][j]:X(1)～X(i)まで調べたときに、X(i)がjになる組み合わせの数
	vector<vector<ll>> dp(N+1, vector<ll>(11, 0));
	for(i = 1; i <= 9; i++) dp[1][i] = 1;  // dp初期設定 X(1)は1～9どれも条件を満たす

	for(i = 2; i <= N; i++)
	{
		for(j = 1; j <= 9; j++)
		{
			// dp[][10]が使えるようにしているので、j=9のときも同じ計算でOK
			dp[i][j] = (dp[i-1][j-1] + dp[i-1][j] + dp[i-1][j+1]) % MOD;
		}
	}

	int count = 0;
	for(auto a : dp[N])
	{
		count = (count+a)%MOD;
	}
	cout << count << endl;

	return 0;
}
