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

// ABC175 https://atcoder.jp/contests/abc175

/*
 * 動的計画法で解く。
 * 
 * dp[r][c][k=4]を以下のように定義する。
 *   マス(r,c)にいて、
 *   r行目にてアイテムをk個拾っているときの
 *   価値の最大値
 * 
 * dpの遷移は以下のようになる。
 * ・上から来た場合  (i,j)->(i+1,j)への移動
 *   行ごとのアイテム取得数はリセットされるので、
 *   dp[i][j][0] = chmax(dp[i][j][0], dp[i-1][j][*])  となる。
 * ・左から来た場合  (i,j)->(i,j+1)への移動
 *   行ごとのアイテム取得数は継続するので、
 *   dp[i][j][k] = chmax(dp[i][j][k], dp[i][j-1][k])  となる。
 * ・アイテムの取得
 *   k=1～3について、
 *   dp[i][j][k] = chmax(dp[i][j][k], dp[i][j][k-1])  となる。
 *   アイテムが無いマスにも価値0のアイテムがあるとすれば、実装が簡易になる。
 *   また、in-placeで更新するため、kが大きい方から更新する必要がある。
 * 
 * dp[R][C][*]の最大値が答。
 */

int main(void)
{
	ll i, j, k;
	ll R, C, K; cin >> R >> C >> K;
	vector<vector<ll>> vl(R+5, vector<ll>(C+5, 0));  // val[R][C]
	for(i = 0; i < K; i++)
	{
		ll r, c, v; cin >> r >> c >> v;
		vl[r][c] = v;
	}
	vector dp(R+5, vector(C+5, vector<ll>(4, 0)));  // dp[R][C][4]
	for(i = 1; i <= R; i++)
	{
		for(j = 1; j <= C; j++)
		{
			for(k = 0; k < 4; k++) dp[i][j][0] = max(dp[i][j][0], dp[i-1][j][k]);  // 上から
			for(k = 0; k < 4; k++) dp[i][j][k] = max(dp[i][j][k], dp[i][j-1][k]);  // 左から
			// アイテムを取る
			for(k = 3; k >= 1; k--)
			{
				dp[i][j][k] = max(dp[i][j][k], dp[i][j][k-1] + vl[i][j]);
			}
		}
	}

	ll ans = 0;
	for(k = 0; k < 4; k++)
	{
		ans = max(ans, dp[R][C][k]);
	}
	cout << ans << endl;

	return 0;
}
