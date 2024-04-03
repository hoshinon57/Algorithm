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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * DP遷移を累積和で高速化する。
 * 
 * dp[i][j]を以下のように定義する。
 *   人iまで見て、 (i=1～N)
 *   そこまでにトータルでj個のアメを配るときの、 (j=0～K)
 *   組み合わせの総数
 * 
 * DP遷移は
 *   dp[i][j] = Σdp[i-1][j-x] (x:0～a[i])
 * となるが、愚直にやるとO(NK^2)となりTLE.
 * 
 * Σの部分は連続した部分の和であることから、ここを累積和で高速化することでACできる。
 */

int main(void)
{
	// 1-indexed
	ll i, j;
	ll N, K; cin >> N >> K;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<vector<ll>> dp(N+1, vector<ll>(K+1, 0));  // dp[N+1][K+1]
	dp[0][0] = 1;

	for(i = 1; i <= N; i++)  // 人i
	{
		vector<ll> sum(K+1+1);  // K+1個の要素の累積和なので、さらに+1
		for(j = 0; j < K+1; j++)  // 累積和の計算
		{
			sum[j+1] = sum[j] + dp[i-1][j];
		}
		for(j = 0; j <= K; j++)  // ここまでに配ったアメの総数 j個
		{
			ll l = max(j-a[i], (ll)0);
			ll r = j;  // [l,r]
			dp[i][j] = sum[r+1] - sum[l];
			dp[i][j] %= MOD;
		}
	}
	cout << dp[N][K] << endl;

	return 0;
}
