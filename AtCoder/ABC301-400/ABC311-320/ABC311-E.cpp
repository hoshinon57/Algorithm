#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC311 https://atcoder.jp/contests/abc311

/*
 * コンテスト中に解けず、解説を見た。
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   マス(i,j)を右下とするような正方形の数
 * 
 * するとdp[i][j]は以下のように決まる。
 *   マス(i,j)が穴であれば0.
 *   そうでなければ、min({dp[i-1][j], dp[i][j-1], dp[i][j]})+1.
 * 
 * 例えばdp[i][j]が3になるならば、
 *   上のマス  (i-1,j  )を右下とする正方形の数は2以上。
 *   左のマス  (i  ,j-1)を右下とする正方形の数は2以上。
 *   左上のマス(i-1,j-1)を右下とする正方形の数は2以上。
 * であることをイメージすると分かりやすい。
 * 
 * dp[*][*]の総和が答。
 * 
 * ※「最大正方形」で検索すると、解法がすぐにヒットした
 *   https://qiita.com/H20/items/884551b4965739176afc
 */

int main(void)
{
	// 0-indexed
	ll i, j;
	ll H, W, N; cin >> H >> W >> N;
	vector<vector<bool>> hole(H, vector<bool>(W, false));  // hole[H][W] 穴ならtrue
	for(i = 0; i < N; i++)
	{
		ll a, b; cin >> a >> b;
		a--; b--;
		hole[a][b] = true;
	}

	vector<vector<ll>> dp(H, vector<ll>(W, 0));  // dp[H][W]  そのマスを右下とする正方形の数
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(hole[i][j])
			{
				dp[i][j] = 0;
				continue;
			}
			if(i == 0 || j == 0)  // 端 (配列の範囲外アクセスを防ぐ)
			{
				dp[i][j] = 1;
				continue;
			}
			dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;
		}
	}

	ll ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			ans += dp[i][j];
		}
	}
	cout << ans << endl;
	
	return 0;
}
