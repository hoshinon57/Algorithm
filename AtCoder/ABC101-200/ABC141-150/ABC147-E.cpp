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

// ABC147 https://atcoder.jp/contests/abc147

/*
 * 動的計画法で解く。
 * 
 * まず、dp[h][w][i]を以下のように定義する。
 *   マス(h,w)にいるときに、"偏り"をiにできるならば1
 * 
 * AおよびBの値には意味が無くて、必要な情報はA,Bの差。
 * 例えば差が3であれば、そのマスで偏りを3増減できる。
 * また、「赤に塗られた数の和」-「青に塗られた数の和」は、例えば
 *   {6, 2, -2, -6}
 * のように正負で鏡写しのようになってそうと分かる。
 * よって計算途中で負の側は考えなくてよい。
 * 
 * また移動回数の最大がH+W=160で、1回あたりの差分の最大が80であるため、
 * (160/2)*80=6400よりも大きくなった偏りは無視してよい。
 * 6400より大きくなると、この後にどういう遷移をしても、これより偏りが小さい要素が存在するため。
 * 
 * 配るDPを用いて、遷移は以下となる。
 *   dp[h][w][i]=1のとき、
 *   (1)下方向
 *     dp[h+1][w][ i+|A-B| ] = 1
 *     dp[h+1][w][|i-|A-B||] = 1
 *   (2)右方向
 *     dp[h][w+1][ i+|A-B| ] = 1
 *     dp[h][w+1][|i-|A-B||] = 1
 * 実装では配列の要素数に余裕を持たせることで、範囲外判定を省略している。
 * 
 * 偏りの最小値を求めたいので、
 * dp[H-1][W-1][*]を0から見ていって最初に1になった部分が答。
 */

int a[80][80], b[80][80];

// 6400よりも大きくなった偏りは無視してよい(前述参照)
int dp[85][85][6400+100];  // +100は、偏り6400から+80しても範囲外アクセスにならないように

int main(void)
{
	int i, j, k;
	int H, W; cin >> H >> W;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> a[i][j];
	}
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> b[i][j];
	}

	dp[0][0][abs(a[0][0]-b[0][0])] = 1;  // DP初期値
	// 配るDP
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			for(k = 0; k <= 6400; k++)  // 偏りk
			{
				if(dp[i][j][k] == 0) continue;
				// 以下、dp[i][j][k] = 1
				int nxt, d;
				// 下方向
				d = abs(a[i+1][j] - b[i+1][j]);
				nxt = k + d;
				dp[i+1][j][nxt] = 1;
				nxt = abs(k - d);
				dp[i+1][j][nxt] = 1;
				// 右方向
				d = abs(a[i][j+1] - b[i][j+1]);
				nxt = k + d;
				dp[i][j+1][nxt] = 1;
				nxt = abs(k - d);
				dp[i][j+1][nxt] = 1;
			}
		}
	}

	int ans = 0;
	// 偏りの最小値を求めたいので、0から見ていって最初に1になった部分が答
	for(i = 0; i <= 6400; i++)
	{
		if(dp[H-1][W-1][i] == 1)
		{
			ans = i;
			break;
		}
	}
	cout << ans << endl;

	return 0;
}
