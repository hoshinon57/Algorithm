#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC327 https://atcoder.jp/contests/abc327

/*
 * 自力で解けず、解説を見た。
 * 動的計画法で解く。
 * 
 * レートRの計算式について、kを固定したときに変化するのは減算左側の分子のみ。
 * k=1～Nそれぞれ、ここの最大値を求めればよく、これをDPで求める。
 * 
 * dp[i][j]を以下のように定義する。
 *   i回目のコンテストについて着目しており、 (i:1～N+1)
 *   i回目含めて残りj回を選ぶときの、 (j:0～N)
 *   レート計算式で変化する分子部分の最大値
 * 
 * DP遷移はiをN側から求めていく形になる。
 * dp[i][j]は
 *   i回目を選ばない場合：dp[i+1][j]
 *   i回目を選ぶ場合：dp[i+1][j-1] + 0.9^(j-1)
 * の最大値となる。
 * 
 * k個選ぶ場合の最大値は dp[1][k]～dp[N][k] の最大値となるので、
 * これとkごとに固定部分を使って「k個選ぶ場合のレート最大値」が求まり、
 * これをk=1～Nそれぞれについて求め、その最大値が答となる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・全てのkについて「k回コンテストに参加する場合の最大値」を求めれば良さそう、というのはイメージできた。
 *   ただkについての最大値を求める方法が思いつかず、「貪欲？」となってた。
 *   ⇒貪欲かどうかで悩むなら、DPを考えてみるべき。
 * ・解説をみてDPとだけ知った後、DPの遷移式を思いつくのに時間がかかった。
 *   いくつかのkについてレート計算式を書いていけば、もっと早くたどり着けたかもしれない。
 */

int main(void)
{
	int i, j;
	ll N; cin >> N;
	vector<ll> p(N+1); for(i = 1; i <= N; i++) {cin >> p[i];}

	vector<vector<double>> dp(N+2, vector<double>(N+1, -INF64));  // dp[N+2][N+1]
	dp[N+1][0] = 0.0;
	for(i = N; i > 0; i--)
	{
		for(j = 0; j <= N; j++)
		{
			// i回目を選ばない
			dp[i][j] = max(dp[i][j], dp[i+1][j]);

			// i回目を選ぶ
			if(j-1 >= 0)
			{
				double t = pow((double)0.9, j-1) * p[i];
				dp[i][j] = max(dp[i][j], dp[i+1][j-1]+t);
			}
		}
	}

	double ans = -INF64;
	for(int k = 1; k <= N; k++)  // k個選ぶ
	{
		double d1, d2, d3;
		d1 = -INF64;
		for(i = 1; i <= N; i++)
		{
			d1 = max(d1, dp[i][k]);
		}
		d2 = 0;
		for(i = 1; i <= k; i++)
		{
			d2 += pow((double)0.9, k-i);
		}
		d3 = (double)1200 / sqrt(k);
		ans = max(ans, d1/d2-d3);
	}
	cout << std::fixed << std::setprecision(15);
	cout << ans << endl;

	return 0;
}
