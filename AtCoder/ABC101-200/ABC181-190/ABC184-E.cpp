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
 * 幅優先探索で解く。
 * 
 * テレポーターを除くと、普通のグリッドグラフ上の探索となる。
 * テレポーターについては、2次元のvectorを用いて
 *   tele['a'] =  {{1,3}, {10,10}, {30,5}, ...}
 * のように各文字のテレポーターの場所を保持しておく。
 * すると現在地にテレポーターがあれば、tele[]の一覧に対してコスト1で移動できる。
 * 
 * ただしこれだけではTLEとなった。
 * 全マスに同じ文字のテレポーターがある場合に、毎回大量の探索が起きるため。
 * 
 * ある文字についてテレポーターは2回以上使うことは無いので、
 * 「この文字のテレポーターを使ったか？」のフラグを追加することでACできた。
 */

vector<string> mp;
vector<vector<pair<int,int>>> tele;

// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点にBFS.
// distは要素数[H][W]で、未探索部分はINF32で埋めていること。
// 探索結果として、distに(sy,sx)を起点(=0)とした距離を格納する。
void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist)
{
	queue<pair<int,int>> que;
	vector<bool> used(26, false);  // used['x']:x文字のワープを使ったか

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
			que.push({ny, nx});
		}
		if(mp[y][x] >= 'a' && mp[y][x] <= 'z')
		{
			int c = (int)mp[y][x] - 'a';
			if(!used[c])  // あるアルファベットについて、2回以上のワープは無意味
			{
				used[c] = true;
				for(auto &e : tele[c])
				{
					int ny = e.first;
					int nx = e.second;
					if(dist[ny][nx] != INF32) continue;
					dist[ny][nx] = dist[y][x] + 1;
					que.push({ny, nx});
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
	bfs_grid_graph(H, W, sy, sx, dist);

	int ans = -1;
	if(dist[gy][gx] != INF32) ans = dist[gy][gx];
	cout << ans << endl;

	return 0;
}
