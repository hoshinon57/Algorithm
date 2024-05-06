#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題33より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc088/tasks/abc088_d

/*
 * 幅優先探索で解く。
 * 
 * X:白で塗られているマス数
 * Y:スタートからゴールまでの最短距離
 * としたときに、X-Yが答となる。
 * 
 * よって入力時にXを計算しておき、
 * Yはスタートから各マスへの最短距離を幅優先探索で求めればよい。
 * 
 * 計算量はO(H*W)となる、はず。
 * ※幅優先探索の計算量は、グラフの頂点をV, 辺をEとしたときにO(V+E)であり、
 *   V:H*W
 *   E:H*W*4 (各マスから、最大で上下左右の4方向に辺が出る)
 *   となるため。
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int H, W;
	cin >> H >> W;
	vector<vector<bool>> wall(H, vector<bool>(W, false));  // wall[i][j]:壁ならtrue (ただしiがY方向)
	vector<vector<int>> dist(H, vector<int>(W, INF32));  // dist[i][j]:スタートから(i,j)までの距離 (ただしiがY方向)
	int white_count = 0;
	for(i = 0; i < H; i++)
	{
		string str;
		cin >> str;
		for(j = 0; j < W; j++)
		{
			// 各マスごとに、黒ならwall[]をtrueに、白ならカウントをインクリメント
			if(str[j] == '#') wall[i][j] = true;
			else white_count++;
		}
	}

	using pii = pair<int,int>;  // {Y方向,X方向}
	queue<pii> que;
	dist[0][0] = 0;  // スタート地点
	que.push({0,0});

	int dy[] = {1, -1, 0,  0};
	int dx[] = {0,  0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();

		for(i = 0; i < 4; i++)  // 上下左右4方向を探索
		{
			int next_y = y + dy[i];
			int next_x = x + dx[i];
			// 移動先が範囲外、または壁ならスルー
			if(next_x < 0 || next_x >= W || next_y < 0 || next_y >= H) continue;
			if(wall[next_y][next_x]) continue;

			if(dist[next_y][next_x] > dist[y][x] + 1)
			{
				dist[next_y][next_x] = dist[y][x] + 1;
				que.push({next_y, next_x});
			}
		}
	}

	if(dist[H-1][W-1] != INF32)
	{
		cout << white_count - (dist[H-1][W-1]+1) << endl;  // スタート地点をdist=0としたため、+1が必要
	}
	else
	{
		cout << -1 << endl;
	}

	return 0;
}
