#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <tuple>
#include <deque>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ARC005 https://atcoder.jp/contests/arc005/

/*
 * 0-1 BFSで解く。
 * 参考：
 *   https://betrue12.hateblo.jp/entry/2018/12/08/000020
 *   https://drken1215.hatenablog.com/entry/2021/07/30/024800
 * 
 * 移動先が道ならコスト0、壁ならコスト1とみなせる。
 * 上記URLを参考に、両端キューを用いた0-1 BFSで解く。
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int H, W;
	cin >> H >> W;
	vector<vector<int>> map(H, vector<int>(W));  // 0なら通れる、1なら壁
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

	deque<pair<int,int>> deq;  // {y,x}
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // スタートからそのマスまで、何回壁を壊せば行けるか
	// スタート地点の初期化
	dist[sy][sx] = 0;
	deq.push_back(make_pair(sy, sx));

	int dy[4] = {1, -1, 0, 0};
	int dx[4] = {0, 0, 1, -1};
	while(!deq.empty())
	{
		int y = deq.front().first;
		int x = deq.front().second;
		deq.pop_front();

		// 4方向それぞれ探索する
		for(i = 0; i < 4; i++)
		{
			int ny = y+dy[i];  // 移動先のy,x座標
			int nx = x+dx[i];
			// 移動先のコスト
			// 移動先が道か壁かでコストが増えるかどうかが決まるため、まずは移動前の地点のコストを設定しておく
			int nd = dist[y][x];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;  // 範囲外

			if(map[ny][nx] == 0)  // 移動先が道
			{
				// 壁を壊す必要はないので、ndのインクリメントは不要
				
				// マス(ny, nx)に来るまでに壊す必要のある壁を、より少なく更新できるか
				// 更新できるならキューに積む
				if(dist[ny][nx] > nd)
				{
					dist[ny][nx] = nd;
					// ndのインクリメントが無かった場合は(移動コストが0)、先頭に追加する
					deq.push_front(make_pair(ny, nx));
				}
			}
			else  // 移動先が壁
			{
				nd++;  // [y][x]から[ny][nx]に移動するにあたり、壁を壊す必要がある
				if(nd > 2) continue;  // 壁を壊せるのは2回まで
				if(dist[ny][nx] > nd)
				{
					dist[ny][nx] = nd;
					// ndのインクリメントがあった場合は(移動コストが1)、末尾に追加する
					deq.push_back(make_pair(ny, nx));
				}
			}
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
