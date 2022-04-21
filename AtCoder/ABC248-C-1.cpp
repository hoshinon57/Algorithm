#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// ABC248 https://atcoder.jp/contests/abc248

// ABC248 C問題(解説を見た後の作成)
int main(void)
{
	int N, M, K;
	cin >> N >> M >> K;

	// dp[N+1][K+1]
	vector<vector<long long>> dp(N+1, vector<long long>(K+1, 0));
	dp[0][0] = 1;

	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < K; j++)
		{
			for(int h = 1; h <= M; h++)
			{
				if( j+h > K ) break;
				dp[i+1][j+h] = (dp[i+1][j+h]+dp[i][j])%998244353;
			}
		}
	}

	long long total = 0;
	for(int i = 1; i <= K; i++)
	{
		total = (total+dp[N][i]) % 998244353;
	}
	cout << total << endl;

	return 0;
}
