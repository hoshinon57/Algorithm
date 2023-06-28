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
const ll MOD = 998244353;

// ABC307 https://atcoder.jp/contests/abc307

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   人iまで見たときに、  (i=0～N-1)
 *   人iに渡す整数が、j=0:人0と同じ値, j=1:人0と異なる値
 *   のときの渡し方
 * 
 * 遷移式は以下。
 *   dp[i][0]:
 *     人0と同じ値を渡すので、人iへの渡し方は1通りしかない。
 *     よってdp[i-1][1]となる。 (dp[i-1][0]は使えない)
 *   dp[i][1]:
 *     dp[i-1][0]*(M-1) + dp[i-1][1]*(M-2) となる。
 *     前者は、人(i-1)以外の数(※)が使えるので M-1 通り。  (※)人(i-1)と人0の持つ数は同じ
 *     後者は、人(i-1)と人0以外の数が使えるので M-2 通り。
 * 
 * 答はdp[N-1][1].
 */
int main(void)
{
	int i;
	ll N, M; cin >> N >> M;
	vector<vector<ll>> dp(N, vector<ll>(2, 0));  // dp[N][2]

	dp[0][0] = M;
	for(i = 1; i < N; i++)
	{
		dp[i][0] = dp[i-1][1];

		dp[i][1] = dp[i-1][0] * (M-1);
		dp[i][1] %= MOD;
		dp[i][1] += dp[i-1][1] * (M-2);
		dp[i][1] %= MOD;
	}
	cout << dp[N-1][1] << endl;

	return 0;
}
