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

// ABC231 https://atcoder.jp/contests/abc231

/*
 * 桁DP(?)で解く。
 * 
 * まず入力例1の
 *   3 87
 *   1 10 100
 * のようなケースにて、
 * 87の1の位はA1=1でのみやりくりでき(7枚払うか、1つ上の硬貨で払ってお釣りを3枚もらうか)、
 * 10の位はA2=10でのみやりくりできると分かる。
 * 
 * dp[i][j]を以下のように定義する。
 *   i種類目の硬貨まで見て、(i=1～N)
 *   硬貨A(i+1)以上で払うぶんの端数について
 *   j=0:硬貨Aiで端数をちょうど払う, j=1:1つ上の硬貨に支払いを任せる(お釣りをもらう)  ときの、
 *   受け渡しする硬貨の最小枚数
 * 
 * 硬貨Aiは何枚まで使えるかを考えると、n=A(i+1)/Ai として 0～n-1 枚までとなる。
 * n枚はらうなら硬貨A(i+1)に任せた方が良いという考え。
 * 
 * Aiで端数をちょうど支払う場合は、d0=X%A(i+1) / a[i] 枚支払い、
 * 1つ上の硬貨に任せる場合は、 d1=n-d0 枚のお釣りを受け取る。
 * これがdp[i-1][0]からの遷移となる。
 * 
 * dp[i-1][1]からの遷移は、A(i-1)では支払わずにAiに任せているので、
 *   d0は1つ増え(支払う枚数が増える)、
 *   d1は1つ減る(お釣りが減る)
 * ことになる。
 * 
 * 答はdp[N][0].
 * 最後の硬貨まで使ってdp[N][1]はありえないため。
 */

int main(void)
{
	int i;
	ll N, X; cin >> N >> X;
	// a[0]とa[N+1]に番兵
	vector<ll> a(N+2); for(i = 1; i <= N; i++) {cin >> a[i];}
	a[N+1] = INF64;

	vector<vector<ll>> dp(N+1, vector<ll>(2, INF64));  // dp[N+1][2]
	// DP初期値
	// 番兵として硬貨A1の手前にdp[0]を置く この時点でdp[0][1]はありえないためINF
	dp[0][0] = 0;
	dp[0][1] = INF64;

	// もらうDP
	for(i = 1; i <= N; i++)  // i種類目の硬貨
	{
		ll n;  // 硬貨Aiを、0～(n-1)枚まで使える
		if(i != N)
		{
			// 例えばA2=10, A1=1の場合、硬貨A1を使う意味があるのは 10-1=9枚 まで
			// 10枚以上は硬貨A2に任せればよいため
			n = a[i+1] / a[i];
		}
		else
		{
			n = X / a[i];  // 最上位の硬貨の場合、ここで全部払ってしまう
		}
		ll d0 = X % a[i+1] / a[i];  // Aiを使える端数について、ちょうど支払う場合に使う枚数
		ll d1 = n - d0;  // AiではなくA(i+1)に支払いを任せる場合について、Aiのお釣りの枚数
		dp[i][0] = dp[i-1][0] + d0;
		dp[i][1] = dp[i-1][0] + d1;

		// dp[i-1][1]からの遷移
		dp[i][0] = min(dp[i][0], dp[i-1][1] + 1 + d0);  // A(i-1)ではAiに支払いを任せるとしているので、ここで1枚多く払う
		dp[i][1] = min(dp[i][1], dp[i-1][1] - 1 + d1);  // A(i-1)ではAiに支払いを任せるとしているので、ここでのお釣りは1枚減る
	}
	cout << dp[N][0] << endl;  // 最後の硬貨まで使ってdp[N][1]はありえない

	return 0;
}
