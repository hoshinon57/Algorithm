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

// ABC184 https://atcoder.jp/contests/abc184

/*
 * 0-1 BFSで解く。
 * 公式解説を参考にした。
 * https://atcoder.jp/contests/abc184/editorial/352
 * 
 * 超頂点a,b,...zを用意し、
 *   テレポーターのあるマス -> 超頂点  へコスト1の辺
 *   超頂点 -> テレポーターのあるマス  へコスト0の辺
 * を張る。
 * キューには座標を積むが、超頂点の場合は{-1, (テレポーターの種類)}とすることで、
 * キュー取り出し時に区別できるようにした。
 */

vector<string> mp;
vector<vector<pair<int,int>>> tele;

// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点に0-1 BFS.
// distは要素数[H][W]で、未探索部分はINFで埋めていること。
// 探索結果として、distに(sy,sx)を起点(=0)とした距離を格納する。
void bfs01_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist)
{
	deque<pair<int,int>> que;
	vector<vector<bool>> seen(H, vector<bool>(W, false));  // 処理済みならtrue
	vector<int> dist_s(26, INF32);  // 超頂点

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

		// yが-1なら超頂点、それ以外は通常のマス
		if(y != -1)
		{
			// 同じ頂点が「コスト0のケース」と「コスト1のケース」で2回キューに積まれることがあるため、ここで処理を1回に抑える
			// ただしこの処理は無くてもACする
			// あった方が無駄な探索が減るので実行時間は早くなる
			if(seen[y][x]) continue;
			seen[y][x] = true;

			for(int i = 0; i < DIR; i++)
			{
				int ny = y + dy[i];
				int nx = x + dx[i];
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				// 壁など 問題によって書き換える
				if(mp[ny][nx] == '#') continue;

				// 距離を更新できるなら、更新してキューに積む
				if(dist[ny][nx] > dist[y][x] + 1)
				{
					dist[ny][nx] = dist[y][x] + 1;
					que.push_back({ny, nx});
				}
			}
			if(mp[y][x] >= 'a' && mp[y][x] <= 'z')  // テレポーターのマスにいる場合
			{
				int c = (int)mp[y][x] - 'a';
				if(dist_s[c] > dist[y][x] + 1)
				{
					dist_s[c] = dist[y][x] + 1;  // dist_s[]を使って超頂点へ移動
					que.push_back({-1, c});  // {判別用 -1, 文字番号}
				}
			}
		}
		else  // 超頂点
		{
			for(auto &e : tele[x])
			{
				int ny = e.first;
				int nx = e.second;
				if(dist[ny][nx] > dist_s[x])
				{
					dist[ny][nx] = dist_s[x];  // コスト0
					que.push_front({ny, nx});
				}
			}
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
	tele.resize(26);  // tele[(文字番号)] = {{y1, x1}, {y2, x2},...}
	for(i = 0; i < H; i++)
	{
		cin >> mp[i];
		for(j = 0; j < W; j++)
		{
			char c = mp[i][j];
			if(c == 'S')
			{
				sy = i; sx = j;
			}
			else if(c == 'G')
			{
				gy = i; gx = j;
			}
			else if(c >= 'a' && c <= 'z')
			{
				tele[(int)c-'a'].push_back({i, j});  // {y, x}
			}
		}
	}
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[H][W]
	bfs01_grid_graph(H, W, sy, sx, dist);

	int ans = -1;
	if(dist[gy][gx] != INF32) ans = dist[gy][gx];
	cout << ans << endl;

	return 0;
}
