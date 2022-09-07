#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC267 https://atcoder.jp/contests/abc267

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を、
 *   a[]のi番目まで見たときに、  (i:0～N-1)
 *   j個選んだときの、  (j:0～M)
 *   Σ数列の最大値
 * と定義する。
 * 
 * 貰うDPで考えたときに、dp[i][j]への遷移は以下2パターンのうち大きい方となる。
 *   a[i]を選ばないケース：dp[i-1][j]
 *   a[i]を選ぶケース：dp[i-1][j-1]+a[i]*j
 */

int main(void)
{
	ll i, j;
	ll N, M;
	cin >> N >> M;
	vector<ll> a(N);  // 0-indexed
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}
	// ここまで入力

	// dp[i][j]：
	//   a[]のi番目まで見たときに、  (i:0～N-1)
	//   j個選んだときの、  (j:0～M)
	//   Σ数列の最大値
	vector<vector<ll>> dp(N, vector<ll>(M+1, 0));
	// 初期値はdp[0][0]=0, dp[0][1]=A1. それ以外はあり得ないため-INFにしておく
	dp[0][1] = a[0];
	for(i = 2; i <= M; i++)
	{
		dp[0][i] = -INF64;
	}

	for(i = 1; i < N; i++)
	{
		for(j = 1; j <= M; j++)  // j=0, つまりdp[*][0]は常に0
		{
			ll tmp1 = dp[i-1][j];  // i番目を選ばない場合
			ll tmp2 = dp[i-1][j-1] + a[i]*j;  // i番目を選ぶ場合
			dp[i][j] = max(tmp1, tmp2);
		}
	}
	cout << dp[N-1][M] << endl;

	return 0;
}
