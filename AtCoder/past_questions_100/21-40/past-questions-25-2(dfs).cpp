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
 * past-questions-25.cpp とほぼ同じで、以下2点を変更してみた。
 * (1)探索済みの地形はarea[]を海に変更する。
 *    これによりseen[]を使わずとも重複探索をはじくことができる。
 * (2)周囲1マスに海を設定しておく。つまりマップサイズは(h+2)*(w+2)となる。
 *    これによりdfs()での範囲外判定が不要になる。
 *    この場合、main()からの呼び出しにて、(1,1)～(h,w)の範囲での探索にする必要がある点に注意。
 * 
 * これらの実装の方が良いかは好みと思うが、「こういう解法もあるよ」の勉強として実装した。
 */

int w, h;
vector<vector<int>> area;  // area[i][j]:(i,j)が0なら海、1なら陸

// マス(y,x)を起点に深さ優先探索
void dfs(int y, int x)
{
	if(area[y][x] == 0) return;  // 陸ではないためスルー

	area[y][x] = 0;  // 探索済みの地形は海に変えることで、重複探索を防ぐ

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

		// 周囲1マスに海を設定しておき、dfs()での範囲外判定を無くす
		area.assign(h+2, vector<int>(w+2));
		for(i = 1; i <= h; i++)
		{
			for(j = 1; j <= w; j++)
			{
				cin >> area[i][j];
			}
		}

		int answer = 0;
		// 周囲1マスを海にしたため、(1,1)～(h,w)までの探索とする
		for(i = 1; i <= h; i++)
		{
			for(j = 1; j <= w; j++)
			{
				if(area[i][j] == 0) continue;  // 陸ではない
				dfs(i, j);
				answer++;  // dfs()を呼び出した回数が、連結成分数となる
			}
		}
		cout << answer << endl;
	}

	return 0;
}
