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

// ABC142 https://atcoder.jp/contests/abc142

/*
 * 動的計画法(bitDP)で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   鍵iまでみたときに、  (i:1～M)
 *   宝箱の状態をbitごとに 1:開いている/0:開いていない で管理したときに、  (j:0～2^N-1)
 *   必要な費用の最小値
 * 答はdp[M][2^N-1]となる。
 * 
 * 遷移は以下となる。
 * (1)鍵iを使わない場合：
 *    dp[i][j] : 自身とdp[i-1][j]との小さい方
 * (2)鍵iを使う場合：
 *    dp[i][j| (鍵iで開けられる宝箱をbit表記) ] : 自身とdp[i-1][j]+(鍵iの価格) との小さい方
 * 
 * dpの状態数が M*2^N であり、計算量はO(M*2^N).
 */

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> a(M+1), c(M+1);  // 鍵a円、開けられる宝箱のbit表記c
	for(i = 1; i <= M; i++)
	{
		int b; cin >> a[i] >> b;
		for(j = 0; j < b; j++)
		{
			int c_tmp; cin >> c_tmp;
			c_tmp--;
			c[i] |= (1<<c_tmp);
		}
	}

	vector<vector<ll>> dp(M+1, vector<ll>(1<<N, INF64));  // dp[M+1][1<<N]
	dp[0][0] = 0;
	// 以下は見た目もらうDPっぽいが、配るDPで実装している
	for(i = 1; i <= M; i++)
	{
		for(j = 0; j < 1<<N; j++)
		{
			// 鍵iを使わない場合
			dp[i][j] = min(dp[i][j], dp[i-1][j]);

			// 鍵iを使う場合
			ll &dp_nxt = dp[i][j|c[i]];
			dp_nxt = min(dp_nxt, dp[i-1][j]+a[i]);
		}
	}

	ll ans = -1;
	if(dp[M][(1<<N)-1] != INF64) ans = dp[M][(1<<N)-1];
	cout << ans << endl;

	return 0;
}
