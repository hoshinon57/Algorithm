#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC232 https://atcoder.jp/contests/abc232

/*
 * 幅優先探索で解く。
 * 
 * 「そのマスを通ったことがあるか」をseen[][]に保持しておく。(本問題の制約を踏まえると、不要かも)
 * マス(1,1)からスタートし、各マスにて下および右に行けるならキューに積んでいく。
 * ※下のマスが範囲外か、すでに通ったことがあるか、壁なら行けない
 * 
 * マスを(y,x)としたときにx+y-1の最大値が求める答え。
 */
int C[101][101];  // 0なら空きマス、1なら壁
bool seen[101][101];  // そのマスを通ったことがあればtrue
int main(void)
{
	// 1-indexed
	int i, j;
	int H, W;
	cin >> H >> W;
	for(i = 1; i <= H; i++)
	{
		for(j = 1; j <= W; j++)
		{
			char tmp;
			cin >> tmp;
			if(tmp == '.') C[i][j] = 0;
			else C[i][j] = 1;

			seen[i][j] = false;
		}
	}

	queue<pair<int,int>> que;
	que.push(make_pair(1, 1));
	int walk_max = -1;  // 通ることのできるマスの最大値

	while(!que.empty())
	{
		pair<int,int> now = que.front();
		que.pop();
		int x, y;
		x = now.second;
		y = now.first;
		// (1,1)が初期値、かつ初期値も1マスとカウントするため、x+y-1がここまでに通ったマスを指す
		walk_max = max(walk_max, x+y-1);

		// 下方向
		if(y+1 > H || seen[y+1][x] || C[y+1][x] == 1)  // 下方向が範囲外か、すでに通ったことがあるか、壁なら行けない
		{
			;
		}
		else
		{
			seen[y+1][x] = true;
			que.push(make_pair(y+1, x));
		}

		// 右方向
		if(x+1 > W || seen[y][x+1] || C[y][x+1] == 1)
		{
			;			
		}
		else
		{
			seen[y][x+1] = true;
			que.push(make_pair(y, x+1));
		}
	}

	cout << walk_max << endl;

	return 0;
}
