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

// ABC183 https://atcoder.jp/contests/abc183

/*
 * 動的計画法で解く。
 * ※考え方は以下と全く同じだった
 *   https://drken1215.hatenablog.com/entry/2020/11/16/204900
 * 
 * dp[i][j]をマス(i,j)への移動方法の数とすると、dp[i][j]は以下(1)～(3)の和となる。
 *   (1)dp[i][j-1]から左方向へ、壁に当たるまでのdp[i][*]の総和
 *   (2)dp[i-1][j]から上方向へ、壁に当たるまでのdp[*][j]の総和
 *   (3)dp[i-1][j-1]から左上方向へ、壁に当たるまでのdp[*][*]の総和
 * ただしこれをそのまま実装してもO(HW(H+W))となりTLE.
 * 
 * よって累積和の考えを用いて、dp_sumを以下のように定義する。
 *   dp_sum[i][j][0]：上記の(1)
 *   dp_sum[i][j][1]：上記の(2)
 *   dp_sum[i][j][2]：上記の(3)
 * するとdp[i][j] = dp_sum[i][j-1][0] + dp_sum[i-1][j][1] + dp_sum[i-1][j-1][2] となる。
 * dp_sum自体は、dp[i][j]が求まった後に
 *  　dp_sum[i][j][0] = dp_sum[i][j-1][0] + dp[i][j]
 *  　[1],[2]も同様
 * とO(1)で求められる。
 * 
 * 「壁に当たるまで」は、マス(i,j)が壁なら計算をしないことで実現できる。
 * 
 * 実装では判定を簡易にするため、左上方向に番兵を設置している。
 */

ll dp[2005][2005];
ll dp_sum[2005][2005][3];
vector<string> mp;

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	mp.push_back("");
	for(i = 0; i < W+1; i++) mp[0] += '#';  // 上に番兵
	for(i = 0; i < H; i++)
	{
		string str; cin >> str;
		mp.push_back('#' + str);  // 左に番兵
	}

	// もらうDP
	for(i = 1; i <= H; i++)
	{
		for(j = 1; j <= W; j++)
		{
			if(mp[i][j] == '#') continue;

			if(i == 1 && j == 1)
			{
				dp[i][j] = 1;  // DP初期値
			}
			else
			{
				dp[i][j] = dp_sum[i][j-1][0] + dp_sum[i-1][j][1] + dp_sum[i-1][j-1][2];
				dp[i][j] %= MOD;
			}
			dp_sum[i][j][0] = (dp_sum[i][j-1][0] + dp[i][j]) % MOD;
			dp_sum[i][j][1] = (dp_sum[i-1][j][1] + dp[i][j]) % MOD;
			dp_sum[i][j][2] = (dp_sum[i-1][j-1][2] + dp[i][j]) % MOD;
		}
	}
	cout << dp[H][W] << endl;

	return 0;
}
