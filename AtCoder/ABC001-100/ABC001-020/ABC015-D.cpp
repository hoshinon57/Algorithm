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

// ABC015 https://atcoder.jp/contests/abc015

/*
 * dp[n][i][j]: 
 *   n枚目までのスクリーンショットまで見て、
 *   貼り付けた枚数がi枚であり、 (i=0～K)
 *   スクリーンショットの幅がjのときの、 (j=0～W)
 *   重要度の最大値
 * としてDP.
 * 
 * 実装ではdp,ndpとして[n]の次元を持たず、二次元としている。
 * ※意識してなかったが、メモリ制限が256MBなので、三次元だとMLEだったかも。
 */

int main(void)
{
	ll i, j;
	ll W; cin >> W;
	ll N, K; cin >> N >> K;
	vector<vector<ll>> dp(K+1, vector<ll>(W+1, -INF64));
	dp[0][0] = 0;
	for(int n = 0; n < N; n++)
	{
		ll a, b; cin >> a >> b;
		vector<vector<ll>> ndp = dp;
		// もらうDP
		for(i = 0; i <= K; i++)  // 使う枚数i
		{
			for(j = 0; j <= W; j++)  // 幅j  ndp[i][j]
			{
				// n個目のスクリーンショットを使わないケース -> ndp=dpにて済み

				// 使うケース
				ll prev = j-a;
				if(prev >= 0 && i-1 >= 0)
				{
					chmax(ndp[i][j], dp[i-1][prev]+b);
				}
			}
		}
		swap(dp, ndp);
	}

	ll ans = -1;
	// dp[*][*]の最大値が答。
	for(i = 0; i <= K; i++)
	{
		chmax(ans, *max_element(dp[i].begin(), dp[i].end()));
	}
	cout << ans << endl;

	return 0;
}
