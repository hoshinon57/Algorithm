#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ARC005 https://atcoder.jp/contests/arc005/

/*
 * 幅優先探索で解く。
 * 
 * スタートからマス(i,j)まで何回壁を壊せば行けるかを、dist[][]にて管理する。
 * スタート地点から4方向に、壁を壊した回数が2回以下の条件で幅優先探索していく。
 * 
 * 最終的に、ゴールマスのdist[]を見てたどり着けるか判定する。
 */

int H, W;
vector<vector<int>> map;  // map[i][j]:マス(i,j)が0なら通れる、1なら壁
vector<vector<int>> dist;  // dist[i][j]:スタートからマス(i,j)まで、何回壁を壊せば行けるか

// マス(ny, nx)に移動し、キューに積む
// nowDist:(ny,nx)に移動するまでに壁を壊した回数
void update(int ny, int nx, int nowDist, queue<pair<int,int>> &que)
{
	if(ny < 0 || ny >= H || nx < 0 || nx >= W) return;  // 範囲外

	if(map[ny][nx] == 1)  // 壁
	{
		nowDist++;
		if(nowDist > 2) return;  // 壁を壊せるのは2回まで
	}

	// マス(ny, nx)に来るまでに壊す必要のある壁を、より少なく更新できるか
	// 更新できるならキューに積む
	if(dist[ny][nx] > nowDist)
	{
		dist[ny][nx] = nowDist;
		que.push(make_pair(ny, nx));		
	}
}

int main(void)
{
	// 0-indexed
	int i, j;
	cin >> H >> W;
	map.resize(H, vector<int>(W));
	int sy, sx, gy, gx;  // それぞれスタートとゴールのマス
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			char tmp;
			cin >> tmp;
			switch(tmp)
			{
			case 's':
				map[i][j] = 0;
				sy = i;
				sx = j;
				break;
			case 'g':
				gy = i;
				gx = j;
				map[i][j] = 0;
				break;
			case '.':
				map[i][j] = 0;
				break;
			case '#':
				map[i][j] = 1;
				break;
			}
		}
	}
	// ここまで入力

	queue<pair<int,int>> que;  // {y,x}
	dist.resize(H, vector<int>(W, INF32));
	// スタート地点の初期化
	dist[sy][sx] = 0;
	que.push(make_pair(sy, sx));

	int dy[4] = {1, -1, 0, 0};
	int dx[4] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();

		// 4方向それぞれ探索する
		for(i = 0; i < 4; i++)
		{
			update(y+dy[i], x+dx[i], dist[y][x], que);
		}
	}

	// ゴール地点にたどり着いているか
	if(dist[gy][gx] == INF32)
	{
		cout << "NO" << endl;
	}
	else
	{
		cout << "YES" << endl;
	}

	return 0;
}
