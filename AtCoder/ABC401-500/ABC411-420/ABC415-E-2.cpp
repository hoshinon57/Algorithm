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

// ABC415 https://atcoder.jp/contests/abc415

/*
 * 公式解説を参考に、二分探索を使わず(1回の)DPで解く実装。
 *   https://atcoder.jp/contests/abc415/editorial/13490
 * 
 * dp[y][x]を以下のように定義する。
 *   マス(y,x)からはじめてゴールまで倒れずに行けるとき、
 *   はじめに持つコインの枚数の最小値
 * dp[0][0]が答。
 * 
 * これは後ろ(ゴールマス側)から確定していくことができる。
 * ※ちなみに二分探索を使用せず、前からDPは解けない。
 *   https://x.com/kyopro_friends/status/1946765971429150761
 * 
 * 基本的には公式解説を参照だが、
 * 次のマスdp'からもらうとき、dp=max(dp'-A, 0) となる。
 * 0下限にするのは、「はじめに持つコインの枚数が-2枚」とかはできないため。
 * これをもらうDPにて右側、下側それぞれから計算し、小さい方を採用する。
 * 
 * ※実装では少し計算順を変更している。
 *   右下それぞれのdp値のminをまず求めている。
 * 
 * ちなみに、二分探索使用が184ms, 未使用が115msだった。
**/

int main(void)
{
	ll i;
	ll y, x;
	ll H, W; cin >> H >> W;
	vector<vector<ll>> a(H, vector<ll>(W, 0));  // a[H][W]
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++) cin >> a[y][x];
	}
	vector<ll> p(H+W-1); for(i = 0; i < H+W-1; i++) {cin >> p[i];}
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			a[y][x] -= p[y+x];
		}
	}

	vector<vector<ll>> dp(H, vector<ll>(W, 0));  // dp[H][W]
	// 後ろから、もらうDP
	for(y = H-1; y >= 0; y--)
	{
		for(x = W-1; x >= 0; x--)
		{
			ll d = INF64;
			if(y == H-1 && x == W-1) d = 0;
			// 右からもらう
			if(x+1 < W) chmin(d, dp[y][x+1]);
			// 下からもらう
			if(y+1 < H) chmin(d, dp[y+1][x]);

			dp[y][x] = max(d-a[y][x], 0LL);
		}
	}
	cout << dp[0][0] << endl;

	return 0;
}
