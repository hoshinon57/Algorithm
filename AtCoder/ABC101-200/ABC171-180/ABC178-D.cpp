#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC178 https://atcoder.jp/contests/abc178

/*
 * dp[i]:3以上の整数のみを使い、総和がiである数列の数
 * とすると、
 *   dp[0]～[2]=0
 *   i>=3のdp[i]
 *     i単体であるケースでまず1通り
 *     j>=3であるような全てのjについて dp[i-j] を加算
 * でO(S^2)で実装した。
 * 
 * ちょっと式変形すればO(S)でも求められる模様。
 *   https://atcoder.jp/contests/abc178/editorial/101
 *   https://drken1215.hatenablog.com/entry/2020/10/11/211000
 */

int main(void)
{
	int i, j;
	int S; cin >> S;
	vector<ll> dp(S+1, 0);
	for(i = 3; i <= S; i++)
	{
		dp[i] = 1;
		for(j = 3; i-j >= 0; j++)
		{
			dp[i] += dp[i-j];
			dp[i] %= MOD;
		}
	}
	cout << dp[S] << endl;

	return 0;
}
