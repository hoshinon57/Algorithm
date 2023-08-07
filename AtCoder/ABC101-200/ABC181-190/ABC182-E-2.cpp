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

// ABC182 https://atcoder.jp/contests/abc182

/*
 * 公式解説を参考に実装した。
 *   https://atcoder.jp/contests/abc182/editorial/292
 *   https://twitter.com/kyopro_friends/status/1325435500266545152
 * 
 * まず、横方向に着目して探索する。
 * あるマス(i,j)が電球だった場合、以下のように処理する。
 *   その左方向を見ていって、ブロックに当たるまで全て照らされている
 *   その右方向を見ていって、ブロックに当たるまで全て照らされている
 * (i,j)が電球であっても、すでに「照らされている」判定をしたマスはスルーすることで、O(HW)に抑えられる。
 * 
 * 縦方向についても同様に探索し、最後に照らされているマスをカウントすればよい。
 */

int main(void)
{
	// 0-indexed
	int i, j, k;
	int H, W, N, M; cin >> H >> W >> N >> M;
	vector<vector<int>> obj(H, vector<int>(W, 0));  // obj[H][W]  0:何もなし,1:電球,2:ブロック
	vector<vector<bool>> light(H, vector<bool>(W, false));  // light[H][W] そのマスが照らされていればtrue
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		obj[a][b] = 1;
	}
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		obj[a][b] = 2;
	}

	// 横方向
	vector<vector<bool>> seen(H, vector<bool>(W, false));
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			int o = obj[i][j];
			if(o == 0 || o == 2) continue;  // 電球以外はスルー
			if(o == 1 && seen[i][j]) continue;  // 電球でも、すでにチェックしたマスはスルー
			// 左方向をチェック 壁に当たるまで
			for(k = j; k >= 0; k--)  // (i,k)
			{
				if(obj[i][k] == 2) break;
				seen[i][k] = light[i][k] = true;
			}
			// 右方向をチェック 壁に当たるまで
			for(k = j; k < W; k++)
			{
				if(obj[i][k] == 2) break;
				seen[i][k] = light[i][k] = true;
			}
		}
	}

	// 横方向
	seen.assign(H, vector<bool>(W, false));
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			int o = obj[i][j];
			if(o == 0 || o == 2) continue;
			if(o == 1 && seen[i][j]) continue;
			for(k = i; k >= 0; k--)  // (k,j)
			{
				if(obj[k][j] == 2) break;
				seen[k][j] = light[k][j] = true;
			}
			for(k = i; k < H; k++)
			{
				if(obj[k][j] == 2) break;
				seen[k][j] = light[k][j] = true;
			}
		}
	}

	ll ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(light[i][j]) ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
