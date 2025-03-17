#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC054 https://atcoder.jp/contests/abc054

/*
 * dp[i個目まで見て][タイプAの量][タイプBの量] のときのコスト最小値
 * としてナップザックDP.
 *   https://drken1215.hatenablog.com/entry/2019/02/15/205900
 *   https://img.atcoder.jp/abc054/editorial.pdf
 * 
 * これをすぐ思いつけなかったのは反省かな…
**/

int main(void)
{
	ll i, j, k;
	ll N, ma, mb; cin >> N >> ma >> mb;
	vector<ll> a(N), b(N), c(N); for(i = 0; i < N; i++) {cin >> a[i] >> b[i] >> c[i];}

	const ll GMX = 400;  // グラム数最大
	vector<vector<ll>> dp(GMX+1, vector<ll>(GMX+1, INF64));  // dp[GMX+1][GMX+1]
	dp[0][0] = 0;
	for(i = 0; i < N; i++)
	{
		vector<vector<ll>> ndp(GMX+1, vector<ll>(GMX+1, INF64));
		ndp = dp;  // i個目を使わないケース
		// もらうDP
		for(j = 0; j <= GMX; j++)
		{
			for(k = 0; k <= GMX; k++)
			{
				if(j-a[i] >= 0 && k-b[i] >= 0)
				{
					chmin(ndp[j][k], dp[j-a[i]][k-b[i]] + c[i]);
				}
			}
		}
		swap(dp, ndp);
	}

	ll ans = INF64;
	for(i = 1; i <= GMX; i++)
	{
		for(j = 1; j <= GMX; j++)
		{
			// i:j = ma:mb
			if(j*ma == i*mb) chmin(ans, dp[i][j]);
		}
	}
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
