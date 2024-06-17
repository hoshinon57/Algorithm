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

// ABC358 https://atcoder.jp/contests/abc358

/*
 * ほぼ解説の通り。
 *   https://atcoder.jp/contests/abc358/editorial/10226
 * 
 * 自由に移動し、あるマスで留まり続けるのが最善。
 * また「自由に移動」は同じマスには2回以上通る必要はない。
 * そのため最大H*W回自由に動いた後、残りの回数をその場に留まる、というのを全マスについて計算し、
 * その最小値が答となる
 * 
 * 「自由に移動」はBFSっぽいDPで解ける。これが新たな学びだった。
 */

int main(void)
{
	ll i, j;
	ll H, W, K; cin >> H >> W >> K;
	int sy, sx; cin >> sy >> sx;
	sy--; sx--;
	vector<vector<ll>> a(H, vector<ll>(W, 0));  // a[H][W]
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> a[i][j];
	}

	vector<vector<ll>> dp(H, vector<ll>(W, -INF64));  // dp[H][W]
	dp[sy][sx] = 0;
	auto dp_new = dp;
	ll C = min(H*W, K);  // 自由に動き回る回数
	for(i = 0; i < C; i++)
	{
		const int DIR = 5;  // 上下左右と動かないケース
		int dx[DIR] = {1, -1, 0, 0, 0};
		int dy[DIR] = {0, 0, 1, -1, 0};
		for(int y = 0; y < H; y++)
		{
			for(int x = 0; x < W; x++)
			{
				for(j = 0; j < DIR; j++)
				{
					int ny = y + dy[j];
					int nx = x + dx[j];
					if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
					chmax(dp_new[ny][nx], dp[y][x] + a[ny][nx]);
				}
			}
		}
		swap(dp, dp_new);
	}

	// 残りの移動回数は(K-C)回
	// 現在地にとどまる
	ll ans = 0;
	for(int y = 0; y < H; y++)
	{
		for(int x = 0; x < W; x++)
		{
			chmax(ans, dp[y][x] + a[y][x]*(K-C));
		}
	}
	cout << ans << endl;

	return 0;
}
