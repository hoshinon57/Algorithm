#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題84
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2012ho/tasks/joi2012ho4

/*
 * いもす法で解く。
 * 
 * ※以下、座標を(y,x)とし、辺の長さをaと記載する。
 *  問題文とは使う名前が異なることに注意。
 * 
 * past-questions-84.cpp とほぼ同じで、シミュレートのフェーズが異なる。
 * ※以下記事を参考に、格子状のマスで考える。
 *   https://www.ioi-jp.org/joi/2011/2012-ho-prob_and_sol/2012-ho-t4-review.pdf
 *   https://atug.tokyo/?p=458#toc9
 *   https://ki84zvkzz4.hatenablog.com/entry/2022/09/07/150818
 * y,xの二重ループにてシミュレートする実装にした。
 * こちらの方が見通しは各段に良さそう。
 * 
 * past-questions-84.cpp に記載した
 * ＞右下、左下の記録フェーズについて、入力Nの外側に記録するケースがあるため、
 * ＞少し外側から計算する必要がある点に注意。
 * より、xは入力Nの三角形の1マス外側まで見ている。
 * (もしくは、xもNまでループさせても、ループ回数が約2倍になるだけで正しい答は得られるはず)
 * 
 * 計算量は同じくO(M+N^2).
 */

const int ARRAY_MAX = 5000+10;
ll imos_table[ARRAY_MAX][ARRAY_MAX] = {0};  // table[y][x]

int main(void)
{
	int i, j;
	ll N, M;
	cin >> N >> M;
	// いもす法 記録フェーズ
	for(i = 0; i < M; i++)
	{
		int y, x, a;  // 問題文とは使う名前が異なるので注意 座標(y,x)と辺の長さa
		cin >> y >> x >> a;
		imos_table[y][x]++;
		imos_table[y][x+1]--;
		imos_table[y+a+1][x]--;
		imos_table[y+a+1][x+a+2]++;
		imos_table[y+a+2][x+1]++;
		imos_table[y+a+2][x+a+2]--;
	}

	// いもす法 シミュレートのフェーズ
	// 右下、左下、右の3方向に対して実施する
	int x, y;
	// 右下(yとxともに増加)方向へ
	for(y = 0; y <= N; y++)
	{
		for(x = 0; x <= y+1; x++)  // 入力Nの三角形の、1マス外側まで見る
		{
			imos_table[y+1][x+1] += imos_table[y][x];
		}
	}

	// 左下(yのみ増加)方向へ
	for(y = 0; y <= N; y++)
	{
		for(x = 0; x <= y+1; x++)
		{
			imos_table[y+1][x] += imos_table[y][x];
		}
	}

	// 右(xのみ増加)方向へ
	for(y = 0; y <= N; y++)
	{
		for(x = 0; x <= y+1; x++)
		{
			imos_table[y][x+1] += imos_table[y][x];
		}
	}

	// 集計
	ll answer = 0;
	for(i = 1; i <= N; i++)  // y
	{
		for(j = 1; j <= i; j++)  // x
		{
			if(imos_table[i][j] > 0) answer++;
		}
	}
	cout << answer << endl;
	
	return 0;
}
