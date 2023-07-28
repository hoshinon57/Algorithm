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

// ABC129 https://atcoder.jp/contests/abc129

/*
 * 通常の動的計画法。
 * 説明割愛。
 */

int main(void)
{
	// 1-indexed
	int i;
	int N, M; cin >> N >> M;
	vector<bool> live(N+1, true);  // i段目を踏めるならtrue
	for(i = 0; i < M; i++)
	{
		int a; cin >> a;
		live[a] = false;
	}
	vector<ll> dp(N+1, 0);
	dp[0] = 1;

	// もらうDP
	for(i = 1; i <= N; i++)
	{
		if(!live[i]) continue;  // i段目を踏めない
		dp[i] = dp[i-1];
		if(i-2 >= 0) dp[i] += dp[i-2];
		dp[i] %= MOD;
	}
	cout << dp[N] << endl;

	return 0;
}
