#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int MOD = 10000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題42
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2015yo/tasks/joi2015yo_d

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i日目まで終わったときに (i:1～M)
 *   都市jにいる場合の (j:0～N)
 *   疲労度の最小値
 * もらうDPにて、遷移は以下となる。
 *   (1)移動せずにとどまる場合：dp[i][j] <- dp[i-1][j]
 *   (2)移動する場合：dp[i][j] <- dp[i-1][j-1] + 移動コスト
 * (1)(2)のうち小さい方が、dp[i][j]への設定値となる。
 * 
 * 出力する値は、(1)の遷移により、M日目のdp値を出力すればよい。
 * (M日目より前に到着したら、後は待てばよいというイメージ)
 * 
 * 計算量はO(MN).
 */

int main(void)
{
	int i, j;
	int N, M;  // 都市N, 日数M
	cin >> N >> M;
	vector<int> d(N+1);  // d[i]:都市i-1と都市iの距離
	for(i = 1; i <= N; i++) cin >> d[i];
	vector<int> c(M+1);  // c[i]:i日目の天候
	for(i = 1; i <= M; i++) cin >> c[i];
	// dp[i][j]
	//   i日目まで終わったときに (i:1～M)
	//   都市jにいる場合の (j:0～N)
	//   疲労度の最小値
	vector<vector<ll>> dp(M+1, vector<ll>(N+1, INF64));
	dp[0][0] = 0;  // 初期値

	// もらうDP
	for(i = 0; i <= M; i++)  // i日目
	{
		for(j = 0; j <= N; j++)  // 都市j
		{
			ll tmp = INF64;
			// 移動せずにとどまる
			if(i-1 >= 0)
			{
				tmp = min(dp[i-1][j], tmp);
			}
			// 移動する
			if(i-1 >= 0 && j-1 >= 0)
			{
				tmp = min(dp[i-1][j-1] + d[j]*c[i], tmp);  // 移動コストはd[j]*c[i]
			}
			dp[i][j] = min(dp[i][j], tmp);
		}
	}

	// DP遷移計算より、M日目の数値を出力すればよい
	// (M日目より前に到着したら、後は待てばよいというイメージ)
	cout << dp[M][N] << endl;

	return 0;
}
