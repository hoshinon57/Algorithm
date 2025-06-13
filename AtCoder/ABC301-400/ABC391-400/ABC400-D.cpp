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

// ABC400 https://atcoder.jp/contests/abc400

// 0-1 BFS.
// コスト1移動の部分は、手持ちのライブラリの実装を流用して楽した。

vector<string> mp;

// ABC176-D Wizard in Mazeの入力を元に実装している。(https://atcoder.jp/contests/abc176/tasks/abc176_d)
// 特にコスト1の部分について、問題に合わせて書き換えること。
// -----
// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点に0-1 BFS.
// distは要素数[H][W]で、未探索部分はINFで埋めていること。
// 探索結果として、distに(sy,sx)を起点(=0)とした距離を格納する。
void bfs01_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist)
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
			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			// 距離を更新できるなら、更新してキューに積む
			if(dist[ny][nx] > dist[y][x])
			{
				dist[ny][nx] = dist[y][x];
				que.push_front({ny, nx});  // コスト0は先頭に追加
			}
		}
		// 前蹴り コスト1
		for(int ny = y-2; ny <= y+2; ny++)
		{
			for(int nx = x-2; nx <= x+2; nx++)
			{
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				if(ny != y && nx != x) continue;
				if(ny == y && nx == x) continue;
				// 壁など 問題によって書き換える
//				if(mp[ny][nx] == '#') continue;

				// 距離を更新できるなら、更新してキューに積む
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
	mp.resize(H);
	for(i = 0; i < H; i++) cin >> mp[i];
	int sy, sx, gy, gx; cin >> sy >> sx >> gy >> gx;
	sy--;
	sx--;
	gy--;
	gx--;
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]

	bfs01_grid_graph(H, W, sy, sx, dist);
	cout << dist[gy][gx] << endl;

	return 0;
}
