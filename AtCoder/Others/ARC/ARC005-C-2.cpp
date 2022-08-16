#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <tuple>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ARC005 https://atcoder.jp/contests/arc005/

/*
 * (ヒープを用いた)ダイクストラ法で解く。
 * 
 * dist[]など管理するデータはARC005-C.cppとほぼ同じ。
 * ヒープを用いたダイクストラ法のため、
 * キューではなく優先度付きキューに積むのが主な差異。
 * 
 * とはいえ実行時間は
 *   幅優先探索(ARC005-C.cpp)：55ms
 *   ヒープ＆ダイクストラ法：62ms
 * と大差無かった。
 * マスごとに繋がるのが4方向のみ、移動コストも0または1(壁)のみ、など問題の性質によるものと思われる。
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

	// {dist[], y, x}を要素とした優先度付きキュー
	// dist[]が小さい方が優先度が高い
	priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<tuple<int,int,int>>> que;
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // スタートからそのマスまで、何回壁を壊せば行けるか
	// スタート地点の初期化
	dist[sy][sx] = 0;
	que.push(make_tuple(0, sy, sx));

	int dy[4] = {1, -1, 0, 0};
	int dx[4] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int d = get<0>(que.top());
		int y = get<1>(que.top());
		int x = get<2>(que.top());
		que.pop();

		if(d > dist[y][x]) continue;  // キューのデータは古いためスルー

		// 4方向それぞれ探索する
		for(i = 0; i < 4; i++)
		{
			int ny = y+dy[i];
			int nx = x+dx[i];
			int nd = d;
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;  // 範囲外

			if(map[ny][nx] == 1)  // 壁
			{
				nd++;
				if(nd > 2) continue;  // 壁を壊せるのは2回まで
			}

			// マス(ny, nx)に来るまでに壊す必要のある壁を、より少なく更新できるか
			// 更新できるならキューに積む
			if(dist[ny][nx] > nd)
			{
				dist[ny][nx] = nd;
				que.push(make_tuple(nd, ny, nx));
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
