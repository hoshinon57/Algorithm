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

// ABC151 https://atcoder.jp/contests/abc151

// 全マスからBFSして、最長距離の最大値が答。

int ans = 0;
vector<string> mp;

// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点にBFS.
// distは要素数[H][W]で、未探索部分はINF32で埋めていること。
// 探索結果として、distに(sy,sx)を起点(=0)とした距離を格納する。
void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist)
{
	queue<pair<int,int>> que;

	dist[sy][sx] = 0;
	que.push({sy, sx});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();
		for(int i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(dist[ny][nx] != INF32) continue;

			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			dist[ny][nx] = dist[y][x] + 1;
			ans = max(ans, dist[ny][nx]);
			que.push({ny, nx});
		}	
	}
	return;
}

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	mp.resize(H);
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
	}

	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] == '#') continue;  // 壁スタートはNG
			vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
			bfs_grid_graph(H, W, i, j, dist);
		}
	}
	cout << ans << endl;

	return 0;
}
