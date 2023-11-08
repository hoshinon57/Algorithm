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
 * ABC327-E.cppと同じく動的計画法だが、dp[]の持ち方が異なる。
 * 公式解説およびけんちょん氏の解説を元に実装した。
 *   https://atcoder.jp/contests/abc327/editorial/7564
 *   https://drken1215.hatenablog.com/entry/2023/11/06/040822
 * 
 * dp[i][j]を以下のように定義する。
 *   i回目のコンテストまで見て、
 *   それまでにj回参加しているときの、
 *   レート計算式で変化する分子部分の最大値
 * 
 * 3回ないし4回参加したときのレートをR3,R4とすると、
 *   R3 = 0.9^2*Q + 0.9^1*Q + 0.9^0*Q
 *   R4 = 0.9^3*Q + 0.9^2*Q + 0.9^1*Q + 0.9^0*Q
 *      = (R3*0.9) + 0.9^0*Q
 * となる。(Qの添字は省略している)
 * よってi回目に参加するとき、DPの遷移は
 *   chmax(ch[i][j], dp[i-1][j-1]*0.9 + p[i])
 * となる。
 * 
 * 答は、dp[N][1]～dp[N][N]までの各dp[N][k]についてそれぞれk回参加したときのレートを計算し、
 * その最大値となる。
 * 
 * ただしi>=jという制約があり、これを無視して計算すると
 * 0.9を乗算し続けて0に近づいてしまい、間違った値となってしまう(1WA)。
 * ・jのforループの上限をiにする
 * ・dp[i-1][j-1]>=-INF64/2 でガードする (double型なので==判定は避ける)
 * といったケアが必要。
 */

int main(void)
{
	int i, j;
	ll N; cin >> N;
	vector<ll> p(N+1); for(i = 1; i <= N; i++) {cin >> p[i];}

	vector<vector<double>> dp(N+1, vector<double>(N+1, -INF64));  // dp[N+1][N+1]
	dp[0][0] = 0.0;
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= i; j++)  // i>=jという制約
		{
			// i回目に不参加
			dp[i][j] = dp[i-1][j];

			// i回目に参加
			if(j-1 >= 0)
//			if(j-1 >= 0 && dp[i-1][j-1] > -INF64/2)  // jのループをNまでにし、ここで-INF64/2で-INF64判定をするという手法もある
			{
				dp[i][j] = max(dp[i][j], dp[i-1][j-1]*0.9 + p[i]);
			}
		}
	}

	double ans = -INF64;
	for(int k = 1; k <= N; k++)  // k個選ぶ
	{
		double d1, d2, d3;
		d1 = dp[N][k];
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
