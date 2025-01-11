#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC387 https://atcoder.jp/contests/abc387

/*
 * 最短距離なのでBFS.
 * dist[y][x][d]でBFSする。
 * d=0:縦移動で(y,x)に到達、d=1:横移動
 * とすればよい。
 * 
 * BFSの初期値は {sy,sx,0}と{sy,sx,1}両方をキューに入れる。
 */

vector<string> mp;

void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<vector<int>>> &dist)
{
	queue<array<int,3>> que;  // {y, x, dir}

	dist[sy][sx][0] = dist[sy][sx][1] = 0;
	que.push({sy, sx, 0});
	que.push({sy, sx, 1});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		auto ar = que.front();
		int y = ar[0];
		int x = ar[1];
		int di = ar[2];
		que.pop();
		for(int i = 0; i < DIR; i++)
		{
			if(di == 0)  // 前が縦移動
			{
				if(dx[i] != 0) continue;
			}
			else // di = 1
			{
				if(dy[i] != 0) continue;
			}
			int ny = y + dy[i];
			int nx = x + dx[i];
			int ndi = 1-di;
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(dist[ny][nx][ndi] != INF32) continue;

			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			dist[ny][nx][ndi] = dist[y][x][di] + 1;
			que.push({ny, nx, ndi});
		}	
	}
	return;
}

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	mp.resize(H);
	int sy, sx, gy, gx;
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
		for(j = 0; j < W; j++)
		{
			if(mp[i][j] == 'S')
			{
				sy = i;
				sx = j;
			}
			if(mp[i][j] == 'G')
			{
				gy = i;
				gx = j;
			}
		}
	}
	vector dist(H, vector(W, vector<int>(2, INF32)));
	bfs_grid_graph(H, W, sy, sx, dist);
	int ans = min(dist[gy][gx][0], dist[gy][gx][1]);
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
