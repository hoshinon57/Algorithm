#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC383 https://atcoder.jp/contests/abc383

// 多始点BFS.

vector<string> mp;

void bfs_grid_graph(int H, int W, vector<vector<int>> &dist)
{
	queue<pair<int,int>> que;
	for(int y = 0; y < H; y++)
	{
		for(int x = 0; x < W; x++)
		{
			if(mp[y][x] == 'H')
			{
				dist[y][x] = 0;
				que.push({y, x});
			}
		}
	}

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
			que.push({ny, nx});
		}	
	}
	return;
}

int main(void)
{
	int H, W, D; cin >> H >> W >> D;
	mp.resize(H);
	int i, j;
	for(i = 0; i < H; i++) cin >> mp[i];
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]

	bfs_grid_graph(H, W, dist);
	int ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) if(dist[i][j] <= D) ans++;
	}
	cout << ans << endl;

	return 0;
}
