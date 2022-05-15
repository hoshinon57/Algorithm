#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;

// ABC251 https://atcoder.jp/contests/abc251

/*
 * 動的計画法で、以下のようにdp[j][i]を定める。
 *   1～i種目まで行動したときに、  i:1～N
 *   かつi種目で j=0:エサをやらない j=1:エサをやる   j:0,1
 *   としたときの、費用の最小値
 * 
 * DPの遷移として、
 * (i+1)種目でエサをやらないなら、i種目ではエサやりする必要がある。
 * (i+1)種目でエサをやるなら、i種目ではエサをやる/やらないどちらでもOK。
 * 
 * N種目の行動が「動物Nと動物1に」エサをあげるのが厄介。
 * よって1種目でエサをやるケース、やらないケースの2通りを計算する。
 * 1種目でエサをやるケース：
 * 	dp[0][1] = ∞
 *  dp[1][1] = a[1]
 *  N種目はエサをやる/やらないどちらでもOK。
 * 1種目でエサをやらないケース：
 *  dp[0][1] = 0
 *  dp[1][1] = ∞
 *  N種目でエサをやる必要がある。
 */
int main(void)
{
	// 1-indexed
	ll i, j;
	ll N;
	cin >> N;
	vector<ll> a(N+1);
	for(i = 1; i <= N; i++) cin >> a[i];

	// dp[j][i]
	// 1～i種目まで行動したときに、  i:1～N
	// かつi種目で j=0:エサをやらない j=1:エサをやる  j:0,1
	// としたときの、費用の最小値
	vector<vector<ll>> dp(2, vector<ll>(N+1, INF64));

	// 1種目でエサをやるケース
	dp[0][1] = INF64;  // エサをやらないケースは使わないので極大の値に
	dp[1][1] = a[1];
	for(i = 1; i < N; i++)
	{
		// (i+1)種目でエサをやらないなら、i種目ではエサやりする必要がある
		dp[0][i+1] = dp[1][i] + 0;  //(i+1)種目でエサをやらないので+0円
		// (i+1)種目でエサをやるなら、i種目ではエサをやる/やらないどちらでもOK
		dp[1][i+1] = min(dp[0][i], dp[1][i])+a[i+1];

	}
	ll answer = min(dp[0][N], dp[1][N]);

	// 1種目でエサをやらないケース
	for(i = 1; i <= N; i++) dp[0][i] = dp[1][i] = INF64; // 初期化
	dp[0][1] = 0;  // エサをやらないので0円
	dp[1][1] = INF64;  // エサをやるケースは使わないので極大の値に
	for(i = 1; i < N; i++)
	{
		dp[0][i+1] = dp[1][i] + 0;
		dp[1][i+1] = min(dp[0][i], dp[1][i])+a[i+1];
	}
	// 動物1にエサをやっていないので、N種目でエサやりする必要がある
	answer = min(answer, dp[1][N]);

	cout << answer << endl;

	return 0;
}
