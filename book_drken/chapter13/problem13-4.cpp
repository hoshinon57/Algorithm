#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
using namespace std;

// 章末問題13.4

/*
 * 幅優先探索で解く。
 * 
 * 迷路と各マスへの最短距離を2次元配列で表し、
 * 最短距離は-1であれば未探索とする。
 * 
 * スタート地点から探索を開始し、
 * 各マスについて上下左右4方向のを探索していく。
 * 探索先が範囲外や壁、または探索済みであればスルーし、
 * それ以外なら最短距離を更新してキューに積む。
 */

// 0-indexed
int H, W;  // 迷路の縦方向、横方向のマス数
pair<int, int> start_, end_;  // スタート地点とゴール地点 {y方向,x方向}の順

// 引数で指定したマスに対して、未探索であれば最短距離を設定してキューに積む
// y,x:探索先のマス
// maze:対象の迷路
// cost[y][x]:マス(y,x)の最短距離 -1なら未探索
// nowCost:マス(y,x)へと来る前のマスの、最短距離
void maze_update(int y, int x, vector<vector<int>> &maze, vector<vector<int>> &cost, int nowCost, queue<pair<int,int>> &que)
{
	if(y < 0 || y >= H || x < 0 || x >= W) return;  // 範囲外
	if(maze[y][x] == 1) return;  // 壁
	if(cost[y][x] != -1) return;  // 探索済み

	// 距離が短い方から探索していくので、costを大きな値で上書きしてしまう、ということは無い。
	// 無条件でcostに設定してしまって良い。
	cost[y][x] = nowCost+1;
	que.push(make_pair(y, x));
}

int main(void)
{
	// 0-indexed
	int i, j;
	cin >> H >> W;
	cin >> start_.first >> start_.second;
	cin >> end_.first >> end_.second;
	vector<vector<int>> maze(H, vector<int>(W));  // maze[i][j]:(i,j)マスが通れるなら0、壁なら1
	vector<vector<int>> cost(H, vector<int>(W, -1));  // cost[i][j]:スタート地点から(i,j)マスへの最短距離 -1なら未探索
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			char tmp;
			cin >> tmp;
			if(tmp == '.') maze[i][j] = 0;
			else maze[i][j] = 1;
		}
	}

	queue<pair<int,int>> que;
	// スタート地点の設定
	cost[start_.first][start_.second] = 0;
	que.push(make_pair(start_.first, start_.second));

	// 幅優先探索
	while(!que.empty())
	{
		auto m = que.front();
		que.pop();
		int nowCost = cost[m.first][m.second];  // 現在の地点の距離

		// 4方向それぞれ探索する
		maze_update(m.first-1, m.second  , maze, cost, nowCost, que);  // 上
		maze_update(m.first+1, m.second  , maze, cost, nowCost, que);  // 下
		maze_update(m.first  , m.second-1, maze, cost, nowCost, que);  // 左
		maze_update(m.first  , m.second+1, maze, cost, nowCost, que);  // 右
	}

	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			cout << std::setw(3);  // 4桁の0埋め
			if(cost[i][j] != -1) cout << cost[i][j];
			else cout << "--";
		}
		cout << endl;
	}
	cout << "answer:" << cost[end_.first][end_.second] << endl;

	return 0;
}
