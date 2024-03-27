#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * dp[i][j]を以下のように定義する。
 *   i枚目まで投げて、 (i=1～N)
 *   表がj枚出るときの確率 (j=0～N)
 * 答はdp[N][(N+1)/2]～dp[N][N]の総和。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<double> p(N+1); for(i = 1; i <= N; i++) {cin >> p[i];}
	vector<vector<double>> dp(N+1, vector<double>(N+1, 0));  // dp[N+1][N+1]
	dp[0][0] = 1.0;
	// もらうDP
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= N; j++)
		{
			// 裏
			dp[i][j] = dp[i-1][j] * (1.0-p[i]);
			// 表
			if(j-1 >= 0)
			{
				dp[i][j] += dp[i-1][j-1] * p[i];
			}
		}
	}

	double ans = 0.0;
	for(j = (N+1)/2; j <= N; j++)
	{
		ans += dp[N][j];
	}
	cout << std::fixed << std::setprecision(15);
	cout << ans << endl;

	return 0;
}
