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

// ABC420 https://atcoder.jp/contests/abc420

/*
 * dist[y][x][s]として情報を持ってBFS.
 * sは0:スイッチを押した回数が偶数回, 1:奇数回.
 * ※スイッチを押した回数を情報に持つ必要がある。2回押すと最初の状態に戻るので、状態数は2で良い。
 * 
 * BFS処理でsの次の状態nsと、それを用いた判定部分に実装とまどった。反省。
**/

vector<string> mp;

void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<vector<int>>> &dist)
{
	queue<array<int,3>> que;  // {y,x,s}

	dist[sy][sx][0] = 0;
	que.push({sy, sx, 0});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		auto [y,x,s] = que.front();
		que.pop();
		for(int i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;

			int ns = s;
			if(mp[ny][nx] == '?') ns = 1-ns;  // 状態反転
			if(dist[ny][nx][ns] != INF32) continue;

			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;
			if((ns == 0 && mp[ny][nx] == 'x') || (ns == 1 && mp[ny][nx] == 'o')) continue;
			// (当たり前だが)?,x,oマスは同時に踏むことは無いので、
			// 判定のタイミングおよびs,nsのどちらを使うか、などはある程度自由に実装できる。

			dist[ny][nx][ns] = dist[y][x][s] + 1;
			que.push({ny, nx, ns});
		}	
	}
	return;
}

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	int sx, sy, gx, gy;
	mp.resize(H);
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
	int ans = *min_element(dist[gy][gx].begin(), dist[gy][gx].end());
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
