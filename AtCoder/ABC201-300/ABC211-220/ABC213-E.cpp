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

// ABC213 https://atcoder.jp/contests/abc213

/*
 * 0-1 BFSで解く。
 * 
 * 通常の移動をコスト0, パンチによって壊せる壁の部分をコスト1とみなす。
 * 公式解説(https://atcoder.jp/contests/abc213/editorial/2397)のそのままだが、
 * 以下のように自身がTにいるとき、#の部分をパンチで壊せる。
 * 判定は「周囲5x5マスで、マンハッタン距離が3以下のものを壊せる」とするのが良さそう。
 *   .###.
 *   #####
 *   ##T##
 *   #####
 *   .###.
 * 
 * あとは通常の0-1BFS.
 */

vector<string> mp;

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
		// パンチ コスト1
		// 自身の周囲5x5マスについて、マンハッタン距離が3以内の壁を壊せる
		for(int ny = y-2; ny <= y+2; ny++)
		{
			for(int nx = x-2; nx <= x+2; nx++)
			{
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				if(abs(ny-y) + abs(nx-x) > 3) continue;  // マンハッタン距離で3以内のみパンチが届く
				if(mp[ny][nx] != '#') continue;  // 一応、壁じゃない場合はスルーしておく

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
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
	}
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
	bfs01_grid_graph(H, W, 0, 0, dist);

	cout << dist[H-1][W-1] << endl;

	return 0;
}
