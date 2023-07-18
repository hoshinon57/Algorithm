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

// ABC176 https://atcoder.jp/contests/abc176

/*
 * 0-1 BFSで解く。
 * 
 * 上下左右移動はコスト0,
 * ワープはコスト1,
 * で0-1 BFSを行えばよい。
 */

vector<string> mp;

void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist)
{
	deque<pair<int,int>> que;
	vector<vector<bool>> seen(H, vector<bool>(W, false));  // 処理済みならtrue

	dist[sy][sx] = 0;
	que.push_front({sy, sx});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop_front();

		// 同じ頂点が「コスト0のケース」と「コスト1のケース」で2回キューに積まれることがあるため、ここで処理を1回に抑える
		// ただしこの処理は無くてもACする
		// あった方が無駄な探索が減るので実行時間は早くなる
		if(seen[y][x]) continue;
		seen[y][x] = true;

		// 上下左右マス コスト0
		for(int i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(mp[ny][nx] == '#') continue;

			if(dist[ny][nx] > dist[y][x])
			{
				dist[ny][nx] = dist[y][x];
				que.push_front({ny, nx});  // コスト0は先頭に追加
			}
		}
		// ワープ コスト1
		for(int ny = y-2; ny <= y+2; ny++)
		{
			for(int nx = x-2; nx <= x+2; nx++)
			{
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				if(mp[ny][nx] == '#') continue;

				if(dist[ny][nx] > dist[y][x] + 1)
				{
					dist[ny][nx] = dist[y][x] + 1;
					que.push_back({ny, nx});  // コスト1は末尾に追加
				}
			}
		}
	}
	return;
}

int main(void)
{
	int i;
	int H, W; cin >> H >> W;
	int sy, sx; cin >> sy >> sx;  // Ch, Cw
	int gy, gx; cin >> gy >> gx;  // Dh, Dw
	sy--; sx--;
	gy--; gx--;
	mp.resize(H);
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
	}
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
	bfs_grid_graph(H, W, sy, sx, dist);

	int ans = -1;
	if(dist[gy][gx] != INF32) ans = dist[gy][gx];
	cout << ans << endl;

	return 0;
}
