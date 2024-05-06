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

// ABC007 https://atcoder.jp/contests/abc007

/*
 * 桁DPで解く。
 * 
 * まず、区間[1,N]における禁止された数を返す関数をsolve(N)とすると、solve(B)-solve(A-1)が答。
 * ※実装では引数をstring型としている
 * 
 * dp[i][smaller][used]を以下のように定義する。
 *   上からi桁目まで見たときに、  (i=1～)
 *   そこまでの値が、s未満である:smaller=1, sと等しい:smaller=0 であり、
 *   4,9を 使用している:used=1, 未使用:used=0
 *   であるような数の種類数
 * 
 * 桁DPにて以下3パターンの遷移を考える。
 * (1)smaller=1 -> smaller=1
 * (2)smaller=0 -> smaller=0
 * (3)smaller=0 -> smaller=1
 * 
 * (1)(2)の具体的な遷移は実装を参照。
 * 
 * (3)について、i桁目の値をdとすると、使える数字は0～(d-1)のd種類。
 * d<=4であれば使える数字はいずれも4,9以外なので、[i-1]からそのままコピーする。
 * d>4であれば使える数字の中に4がある(※)ので、
 *   dp[i][1][0]はdp[i][0][0]からそれぞれ(d-1)通り。
 *   dp[i][1][1]はdp[i][0][0]から4を使うケースと、dp[i-1][0][1]から自由にd種類使えるケースの和。
 * (※)smaller=0からsmaller=1への遷移のため、9はそもそも使えない
 * 
 * solve(N)について、dp[L][0][1]+dp[L][1][1]が答となる。
 */

ll solve(string s)
{
	int L = (int)s.size();
	ll dp[20][2][2] = {0};
	dp[0][0][0] = 1;  // DP初期値

	for(int i = 1; i <= L; i++)
	{
		int d = s[i-1] - '0';
		// smaller=1 -> smaller=1 への遷移
		dp[i][1][0] += dp[i-1][1][0] * 8;  // 4,9を除く8種類
		dp[i][1][1] += dp[i-1][1][0] * 2 + dp[i-1][1][1] * 10;  // 前者は4,9未使用から4,9使用(2通り) 後者は4,9使用済みなので10通り

		// smaller=0 -> smaller=0 への遷移
		if(d == 4 || d == 9)
		{
			// 4,9未使用および4,9使用済み、それぞれからdp[i][0][1]へのみ遷移
			dp[i][0][1] += dp[i-1][0][0] + dp[i-1][0][1];
		}
		else  // 4,9以外
		{
			// 前からそのままコピー
			dp[i][0][0] += dp[i-1][0][0];
			dp[i][0][1] += dp[i-1][0][1];
		}

		// smaller=0 -> smaller=1 への遷移
		// 使える数字は0～(d-1) のd種類
		if(d <= 4)  // 使える数字はいずれも4,9以外
		{
			dp[i][1][0] += dp[i-1][0][0] * d;
			dp[i][1][1] += dp[i-1][0][1] * d;
		}
		else  // 使える数字のうち、4がある
		{
			dp[i][1][0] += dp[i-1][0][0] * (d-1);  // 4が使えないのでd-1種類
			dp[i][1][1] += dp[i-1][0][0] + dp[i-1][0][1] * d;
		}
	}

	return dp[L][0][1] +dp[L][1][1];
}

int main(void)
{
	ll a, b; cin >> a >> b;
	a--;

	ll ans_a = solve(to_string(a));
	ll ans_b = solve(to_string(b));
	cout << ans_b-ans_a << endl;

	return 0;
}
