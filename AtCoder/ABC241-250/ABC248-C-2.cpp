#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// ABC248 https://atcoder.jp/contests/abc248

// ABC248-C 解説にあった、貰うDPでの実装
int main(void)
{
	int i, j, h;
	int N, M, K;
	cin >> N >> M >> K;

	// dp[N+1][K+1]
	vector<vector<long long>> dp(N+1, vector<long long>(K+1, 0));
	dp[0][0] = 1;
	// dp2[N+1][K+1]
	// dp2[a][b]は、dp[a][0]～dp[a][b]までの和
	vector<vector<long long>> dp2(N+1, vector<long long>(K+1, 0));
	for(i = 0; i <= K; i++)
	{
		dp2[0][i] = 1;
	}

	int max_, min_;
	long long sum;
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= K; j++)
		{
			max_ = j-1;
			if( j-M-1 >= 0 )
			{
				min_ = j-M-1;
				// 減算結果が負になると正しくmod計算できないため、modする数を足しておく
				dp[i][j] = (dp2[i-1][max_] + 998244353 - dp2[i-1][min_])%998244353;
			}
			else
			{
				dp[i][j] = dp2[i-1][max_];
			}
		}
		sum = 0;
		for(j = 1; j <= K; j++)
		{
			sum = (sum+dp[i][j])%998244353;
			dp2[i][j] = sum;
		}
	}

	long long total = 0;
	for(i = 1; i <= K; i++)
	{
		total = (total+dp[N][i]) % 998244353;
	}
	cout << total << endl;

	return 0;
}
