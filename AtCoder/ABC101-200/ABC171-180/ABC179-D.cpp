#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC179d https://atcoder.jp/contests/abc179

/*
 * 動的計画法(配るDP)で解く。
 * 
 * dp[i]を、マスiに行く方法の数として定義する。
 * 単純に移動可能なマスごとに計算すると、入力が区間[L,R]のためO(N^2)となりTLEになってしまう。
 * そのため、いもす法(累積和)的な考え方で解く。
 * 
 * まずDP初期値を dp[1]=1, dp[2]=-1 とする。
 * dp[i]=x, [L1,R1]とすると、
 *   dp[i+L1] にxを加算
 *   dp[i+R1+1] にxを減算
 * して、各iごとに dp[i] += dp[i-1] とすることで、先頭からdp[]を確定させていくことができる。
 */

int main(void)
{
	int i, j;
	int N, K; cin >> N >> K;
	vector<ll> dp(N+1);  // dp[i]:マスiに行く方法の数
	// DP初期値
	dp[1] = 1;
	dp[2] = -1;
	vector<pair<ll,ll>> mov;
	for(i = 0; i < K; i++)
	{
		int l, r; cin >> l >> r;
		mov.push_back({l,r});
	}

	// 配るDP
	for(i = 1; i <= N; i++)
	{
		dp[i] += dp[i-1];
		dp[i] %= MOD;
		for(j = 0; j < K; j++)
		{
			int nxt = i + mov[j].first;
			if(nxt > N) continue;
			dp[nxt] += dp[i];
			dp[nxt] %= MOD;

			nxt = i + mov[j].second + 1;
			if(nxt > N) continue;
			// dp[nxt] -= dp[i];
			dp[nxt] = (dp[nxt] + MOD - dp[i]) % MOD;
		}
	}
	cout << dp[N] << endl;

	return 0;
}
