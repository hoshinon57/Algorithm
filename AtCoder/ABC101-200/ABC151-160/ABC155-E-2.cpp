#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC155 https://atcoder.jp/contests/abc155

/*
 * 桁DP(っぽいやつ)で、上から解いてみる。
 *   https://www.creativ.xyz/abc155-e/
 *   ※上からDPが公式解説と上記URLぐらいだった
 * 
 * dp[j]を以下のように定義する。
 *   i桁目まで見て、  ※dpには持たせていない
 *   j=0:この桁まででぴったり払う, j=1:この桁で1枚多く払う
 *   としたときの、使う枚数の最小値
 * 最下位桁まで処理して、dp[0]が答。
 * 
 * DP遷移はもらうDPで考えてみる。
 * dp->ndpとし、次に見る桁の値をdとする。
 * ndp[0]:
 *   dp[0]からは、この桁でd枚払ってぴったりにするので、+d.
 *   dp[1]からは、この桁では払わずお釣りを(10-d)枚もらうので、+(10-d).
 * ndp[1]:
 *   dp[0]からは、この桁で(d+1)枚払って1枚余らすので、+(d+1).
 *   dp[1]からは、この桁では払わずお釣りを(10-d)枚もらい、その上で1枚余らすので、+(10-d)-1.
 * 
 * 【反省】
 * 最近は桁DPの復習をやっていて、その解き方でやろうとしてドツボにはまってしまった。
 * 桁DPありきではなく、「どういう遷移となるか」の本質を考える必要がある。
 */

int main(void)
{
	int i;
	string s; cin >> s;
	int L = (int)s.size();

	vector<ll> dp(2);
	dp[0] = 0;
	dp[1] = 1;
	// もらうDP
	for(i = 0; i < L; i++)
	{
		vector<ll> ndp(2, INF64);
		const int d = s[i] - '0';
		ndp[0] = min(dp[0]+d, dp[1]+(10-d));
		ndp[1] = min(dp[0]+d+1, dp[1]+(10-d)-1);
		swap(dp, ndp);
	}
	cout << dp[0] << endl;

	return 0;
}
