#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題25
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1160&lang=ja

/*
 * 深さ優先探索で解く。
 * 
 * グラフの連結成分数は、dfs()の呼び出し回数と一致する。
 * dfs()にて周囲8マスへ探索するようにすればよい。
 */

int w, h;
vector<vector<bool>> seen;  // seen[i][j]:(i,j)が探索済みならtrue
vector<vector<int>> area;  // area[i][j]:(i,j)が0なら海、1なら陸

// マス(y,x)を起点に深さ優先探索
void dfs(int y, int x)
{
	if(y < 0 || y >= h || x < 0 || x >= w) return;  // 範囲外
	if(area[y][x] == 0) return;  // 陸ではないためスルー
	if(seen[y][x]) return;  // 探索済みならスルー

	seen[y][x] = true;

	// (y,x)の周囲8マスを探索
	// i,j=0のときの明示的な除外は不要 dfs()呼び出し先のseen[]にて除外されるため
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			dfs(y+i, x+j);
		}
	}
}

int main(void)
{
	int i, j;
	while(true)
	{
		cin >> w >> h;
		if(w == 0 && h == 0) break;

		area.assign(h, vector<int>(w));
		seen.assign(h, vector<bool>(w, false));
		for(i = 0; i < h; i++)
		{
			for(j = 0; j < w; j++)
			{
				cin >> area[i][j];
			}
		}

		int answer = 0;
		for(i = 0; i < h; i++)
		{
			for(j = 0; j < w; j++)
			{
				if(area[i][j] == 0) continue;  // 陸ではない
				if(seen[i][j]) continue;  // 探索済み
				dfs(i, j);
				answer++;  // dfs()を呼び出した回数が、連結成分数となる
			}
		}
		cout << answer << endl;
	}

	return 0;
}
