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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題25
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1160&lang=ja

/*
 * 幅優先探索で解く。
 * 
 * 本問題では、深さ優先探索も幅優先探索も考え方は同じ。
 * グラフの連結成分数はbfs()の呼び出し回数と一致する。
 * bfs()にて周囲8マスへ探索するようにすればよい。
 * 
 * グラフにおける幅優先探索の計算量は、頂点数をV,辺数をEとしてO(V+E)となる。
 * 本問題ではV,Eともにw*hに比例するため、計算量はデータセット1つあたりO(wh)と考えられる。
 */

int w, h;
vector<vector<bool>> seen;  // seen[i][j]:(i,j)が探索済みならtrue
vector<vector<int>> area;  // area[i][j]:(i,j)が0なら海、1なら陸

// {i,j}を起点に幅優先探索を行う
void bfs(int i, int j)
{
	queue<pair<int,int>> que;
	que.push({i, j});  // {y,x}
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();

		// (y,x)の周囲8マスを探索
		// a,b=0のときの明示的な除外は不要 seen[]にて除外されるため
		for(int a = -1; a <= 1; a++)
		{
			for(int b = -1; b <= 1; b++)
			{
				int ny = y+a;
				int nx = x+b;
				if(ny < 0 || ny >= h || nx < 0 || nx >= w) continue;  // 範囲外ならスルー
				if(area[ny][nx] != 1) continue;  // 陸でなければスルー
				if(seen[ny][nx]) continue;  // 探索済みならスルー

				seen[ny][nx] = true;
				que.push({ny, nx});
			}
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
				if(area[i][j] != 1) continue;  // 陸でなければスルー
				if(seen[i][j]) continue;  // 探索済みならスルー

				seen[i][j] = true;
				bfs(i, j);
				answer++;  // dfs()を呼び出した回数が、連結成分数となる
			}
		}
		cout << answer << endl;
	}

	return 0;
}
