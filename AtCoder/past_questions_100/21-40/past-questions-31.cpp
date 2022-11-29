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
const ll MOD = 100000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題31
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2008yo/tasks/joi2008yo_e

/*
 * 幅優先探索で解く。
 * 
 * 実装方針は公式解説の通り。
 * https://www.ioi-jp.org/joi/2011/2012-yo-prob_and_sol/2012-yo-t5/review/2012-yo-t5-review.html
 * 
 * 以下の2フェーズに分けられる。
 * (1)空地のうち、建物の中を通らずに行ける部分を幅優先探索するフェーズ
 * (2)(1)の結果をもとに、各建物のイルミネーションの数をカウントしていくフェーズ
 * 
 * areaやseenなどマップ情報は、周囲に1マスの余白を持たせておく。
 * これにより(1)のBFSにて、最初に{0,0}にのみキューを積めば全て辿ることができる。
 * 
 * (2)の実装は、建物がある部分について、
 * 周囲6マスの「空地、かつ建物の中を通らずに行ける部分」をカウントしていけばよい。
 * この条件は「area[]が0かつseen[]がtrueであること」と言い換えることができる。
 * 
 * 計算量はO(HW)と思われる。
 * 幅優先探索の計算量がO(頂点数＋辺の数)であり、どちらもHWの定数倍であるため。
 */

int W, H;
using vvi = vector<vector<int>>;  // [x][y]
using vvb = vector<vector<bool>>;  // [x][y]
vvi area;  // area[x][y]:{x,y}に建物があれば1
vvb seen;  // seen[x][y]:BFSにて{x,y}を探索済みならtrue

// yが奇数のときと偶数のときそれぞれ、周囲6マスの座標
int dx_odd[6]  = { 0, +1, -1, +1,  0, +1};
int dy_odd[6]  = {-1, -1,  0,  0, +1, +1};
int dx_even[6] = {-1,  0, -1, +1, -1,  0};
int dy_even[6] = {-1, -1,  0,  0, +1, +1};

// 空地のうち、建物の中を通らずに行ける部分を幅優先探索する
void bfs(void)
{
	int i;
	queue<pair<int,int>> que;  // {x,y};

	// 周辺に1マスずつ空地を追加しているため、{0,0}から探索すれば全て辿れる
	que.push({0,0});
	seen[0][0] = true;

	while(!que.empty())
	{
		int x = que.front().first;
		int y = que.front().second;
		que.pop();

		// 周囲6マスを探索
		for(i = 0; i < 6; i++)
		{
			int nx, ny;
			if(y%2 == 1)
			{
				nx = x + dx_odd[i];
				ny = y + dy_odd[i];
			}
			else
			{
				nx = x + dx_even[i];
				ny = y + dy_even[i];
			}
			if(nx < 0 || nx > W+1 || ny < 0 || ny > H+1) continue;  // 範囲外
			if(area[nx][ny] == 1) continue;  // 建物はスルー
			if(seen[nx][ny]) continue;  // 探索済み

			seen[nx][ny] = true;
			que.push({nx, ny});
		}
	}
}

int main(void)
{
	// 1-indexed
	int i, j, k;
	cin >> W >> H;
	// 周辺に1マスずつ空地を用意する よって+2
	area.assign(W+2, vector<int>(H+2, 0));  // 入力が無い周辺は0, 空地にしておく
	seen.assign(W+2, vector<bool>(H+2, false));
	for(i = 1; i <= H; i++)
	{
		for(j = 1; j <= W; j++)
		{
			cin >> area[j][i];
		}
	}

	// 空地のうち、建物の中を通らずに行ける部分を幅優先探索する
	bfs();

	// BFSの結果をもとに、各建物のイルミネーションの数をカウントしていく
	int count = 0;
	for(i = 1; i <= H; i++)  // y
	{
		for(j = 1; j <= W; j++)  // x
		{
			if(area[j][i] == 0) continue;  // 飾るのは建物の部分のみ 空地はスルー

			// 建物がある部分について、周囲6マスの「空地、かつ建物の中を通らずに行ける部分」をカウントする
			// この条件は「area[]が0かつseen[]がtrueであること」と言い換えることができる
			for(k = 0; k < 6; k++)
			{
				int nx, ny;
				if(i%2 == 1)
				{
					nx = j + dx_odd[k];
					ny = i + dy_odd[k];
				}
				else
				{
					nx = j + dx_even[k];
					ny = i + dy_even[k];
				}
				if(area[nx][ny] == 0 && seen[nx][ny]) count++;
			}
		}
	}
	cout << count << endl;

	return 0;
}
