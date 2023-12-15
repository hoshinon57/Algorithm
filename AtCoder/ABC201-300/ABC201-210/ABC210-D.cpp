#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC210 https://atcoder.jp/contests/abc210

/*
 * DPで解く。
 * 
 * まず、2つの駅の座標を(i1,j1), (i2,j2)とし、i1<=i2かつj1<=j2とする。(ただし2点は異なる座標)
 * つまり、(i1,j1)が左上、(i2,j2)が右下にいる形となる。
 * 
 * dp[i][j]を以下のように定義する。
 *   右下側の駅を(i,j)に建てる場合の、費用の最小値
 * 
 * DP遷移は以下となる。
 * (1)(i-1,j)に右下側の駅を建てたが、やっぱり(i,j)に変えるケース
 *    これはdp[i-1][j]からa[i-1][j]を引き、a[i][j]とCを加算する形となる。
 * (2)(i,j-1)に右下側の駅を建てたが、やっぱり(i,j)に変えるケース
 *    (1)と同様。
 * (3)(i-1,j)に左上側の駅を、(i,j)に右下側の駅を建てるケース
 *    これは単純にa[i-1][j]+a[i][j]+Cとなる。
 * (4)(i,j-1)に左上側の駅を、(i,j)に右下側の駅を建てるケース
 *    (3)と同様。
 * dp[i][j]は上記4パターンのうちの最小値となる。
 * 
 * ただし上記だけでは、2つの駅の位置関係が「左下、右上」のケースで正しく計算できない。(1WA)
 * これは入力Aを左右反転し、同じDP計算を行えばよい。
 * 
 * [ACまでの思考の流れ]
 * ・制約より、座標ごとにO(1)で計算したい。
 * ・駅を建てる位置を(i-1,j)から(i,j)に移すとどうなるか？
 *   Aの差分とCを1つ加算することで、O(1)で解けないか？
 * ・(i-1,j)と(i,j)に駅を建てるケースも考慮が必要っぽい。
 * ・上記だと3ケースでWAになる…
 *   よく考えたら駅の位置関係が「左下、右上」のケースで正しく計算できない。
 *   なら左右反転させてもう1回DPすれば良さそう。
 */

ll H, W, C;
ll calc(vector<vector<ll>> &a)
{
	int i, j;
	vector<vector<ll>> dp(H, vector<ll>(W, 0));  // dp[H][W]
	ll ans = INF64;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			ll d = INF64;
			if(i-1 >= 0)
			{
				chmin(d, dp[i-1][j] - a[i-1][j] + a[i][j] + C);  // (i-1,j)に右下の駅を建てたが、(i,j)に移すケース
				chmin(d, a[i-1][j] + a[i][j] + C);  // (i-1,j)に左上に、(i,j)に右下に相当する駅を建てるケース
			}
			if(j-1 >= 0)
			{
				chmin(d, dp[i][j-1] - a[i][j-1] + a[i][j] + C);
				chmin(d, a[i][j-1] + a[i][j] + C);
			}
			dp[i][j] = d;
			chmin(ans, d);
		}
	}
	return ans;
}

int main(void)
{
	int i, j;
	cin >> H >> W >> C;
	vector<vector<ll>> a(H, vector<ll>(W, 0));
	vector<vector<ll>> a_rev(H, vector<ll>(W, 0));  // 左右反転
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			cin >> a[i][j];
			a_rev[i][W-1-j]= a[i][j];
		}
	}
	cout << min(calc(a), calc(a_rev)) << endl;

	return 0;
}
