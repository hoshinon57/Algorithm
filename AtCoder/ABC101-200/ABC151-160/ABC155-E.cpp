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

// ABC155 https://atcoder.jp/contests/abc155

/*
 * 動的計画法で解く。(桁DPで、下の桁から見ていく)
 * 
 * dp[i][j]を以下のように定義する。
 *   下からi桁目までを見たときに、  (i=1～)
 *   i桁目の分の支払いを j=0:この桁で済ませる j=1:上の桁に任せる
 *   ときの使う紙幣の最小数
 * 
 * 例えばN=36のとき、6について
 *   dp[0][0]:貨幣1を6枚使って支払ってしまう お釣りは0枚
 *   dp[0][1]:貨幣1は使わず、貨幣10の支払いに任せる お釣りは10-6=4枚
 * となるイメージ。
 * 
 * もらうDPで、遷移は以下のようになる。(i桁目の値をkとする)
 *   dp[i][0]:
 *     ここでは支払いのみで、お釣りは出ない。
 *     基本はk枚払うが、dp[i-1][1]からの遷移では上の桁で1枚多く払うので、k+1枚。
 *   dp[i][1]:
 *     ここでは支払わない代わりに、お釣りの分がカウントに加算される。
 *     基本は(10-k)枚お釣りをもらうが、dp[i-1][1]からの遷移では上の桁で1枚多く払うことになるので、お釣りは10-(k+1)枚。
 * 
 * 最上位の桁に、明示的に0を入れておく。
 * 答はdp[L-1][0]. (最上位の桁で、いまさら上の位に支払いを任せるのはありえないため、dp[L-1][1]は使わない)
 * 
 * 実装では、最下位の桁にも番兵を入れることで、実装を簡易にした。
 */

int main(void)
{
	int i;
	string s; cin >> s;
	s = '0' + s + '0';  // 前後に番兵
	int L = (int)s.size();
	reverse(s.begin(), s.end());  // 下の桁からDPをしたいので、逆順にしておくと（自分は）記述が楽

	vector<vector<int>> dp(L, vector<int>(2, 0));  // dp[L][2]
	// DP初期値 最下位に番兵として入れた0の部分
	// この時点でj=1はありえないためINFにしておく
	dp[0][0] = 0;
	dp[0][1] = INF32;

	// もらうDP
	for(i = 1; i < L; i++)
	{
		int k = (int)(s[i] - '0');
		int d0, d1;
		d0 = dp[i-1][0] + k;
		d1 = dp[i-1][1] + k+1;
		dp[i][0] = min(d0, d1);

		d0 = dp[i-1][0] + 10-k;
		d1 = dp[i-1][1] + 10-(k+1);
		dp[i][1] = min(d0, d1);
	}
	cout << dp[L-1][0] << endl;

	return 0;
}
