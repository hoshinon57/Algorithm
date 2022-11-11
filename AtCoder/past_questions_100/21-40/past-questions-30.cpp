#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題30
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2011yo/tasks/joi2011yo_e

/*
 * 幅優先探索で解く。
 * 
 * スタート地点から工場1, 工場1から工場2, ..., 工場(N-1)から工場Nへの距離を、
 * それぞれ幅優先探索で求めて、その総和が答になる。
 * よって幅優先探索をN回行えばよい。
 * 
 * 幅優先探索の部分の実装はいわゆる「教科書通りの実装」であり、特筆すべき要素は無い。
 * 
 * 計算量は、1回あたりの幅優先探索がO(HW)でN回実施するため、
 * 全体でO(NHW)となる。
 */

int H, W, N;
vector<int> fy, fx;  // fy[i]:i番目の工場のy座標 ただしfy[0]はスタート地点のy座標とする
vector<vector<bool>> walk;  // walk[y][x]:(y,x)が通れるならtrue

int main(void)
{
	// 座標は0-indexed, 工場は1-indexed(0をスタート地点とするため)
	int i, j;
	cin >> H >> W >> N;
	walk.resize(H, vector<bool>(W, true));
	fy.resize(N+1);  // 0:スタート地点, 1～N:工場
	fx.resize(N+1);
	for(i = 0; i < H; i++)
	{
		string str;
		cin >> str;
		for(j = 0; j < W; j++)
		{
			if(str[j] == 'S')
			{
				fy[0] = i;
				fx[0] = j;
			}
			else if(str[j] == '.') continue;
			else if(str[j] == 'X')
			{
				walk[i][j] = false;
			}
			else  // '1'～'9'
			{
				int tmp = (int)(str[j] - '0');
				fy[tmp] = i;
				fx[tmp] = j;
			}
		}
	}
	// ここまで入力

	// 0->1, 1->2, ..., (N-1)->N までの距離をそれぞれBFSで解き、その総和が答となる
	ll answer = 0;
	for(i = 0; i < N; i++)
	{
		queue<pair<int,int>> que;  // {y,x}
		vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[y][x]

		// 始点は(fy[i], fx[i])
		dist[fy[i]][fx[i]] = 0;
		que.push({fy[i], fx[i]});

		int dy[4] = {1, -1, 0, 0};
		int dx[4] = {0, 0, 1, -1};
		while(!que.empty())
		{
			int y = que.front().first;
			int x = que.front().second;
			que.pop();

			// 上下左右4方向へ探索
			for(j = 0; j < 4; j++)
			{
				int to_y = y + dy[j];
				int to_x = x + dx[j];
				if(to_y < 0 || to_y >= H || to_x < 0 || to_x >= W) continue;  // 範囲外へは進めない
				if(!walk[to_y][to_x]) continue;  // 障害物には進めない

				if(dist[to_y][to_x] == INF32)
				{
					dist[to_y][to_x] = dist[y][x] + 1;
					que.push({to_y, to_x});
				}
			}
		}

		// ゴールは(fy[i+1], fx[i+1])
		answer += dist[fy[i+1]][fx[i+1]];
	}

	cout << answer << endl;

	return 0;
}
