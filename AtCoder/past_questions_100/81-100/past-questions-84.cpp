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
 * https://imoz.jp/algorithms/imos_method.html
 * を参考に、「右方向」「左下方向」「右下方向」の順にいもす法の適用を考えると、
 * 入力で与えられる三角形の上側頂点(y,x)に対して、
 *   (y,x):+1
 *   (y,x+1):-1
 *   (y+a+1,x):-1
 *   (y+a+1,x+a+2):+1
 *   (y+a+2,x+1):+1
 *   (y+a+2,x+a+2):-1
 * の6点をいもす法のテーブルに加減算することになる。
 * これがいもす法の記録フェーズに相当する。
 * 
 * 次はシミュレートのフェーズを実施する。
 * 右下、左下の記録フェーズについて、入力Nの外側に記録するケースがあるため、
 * 少し外側から計算する必要がある点に注意。
 * 具体的には、(y,x)=(0,1),(1,2),(2,3),... ,(N,N+1)を加味する必要がある。
 * (実装では、Nの最大値より少し大きく配列を確保している)
 * 
 * 右下方向のシミュレートでは、(y,x)について
 *   (0,1)を起点に、(1,2),(2,3),..., (N-1,N)へと累積和計算
 *   (1,1)を起点に、(2,2),(3,3),..., (N,N)  へと累積和計算
 *   (2,1)を起点に、(3,2),(4,3),..., (N,N-1)へと累積和計算
 * というループを回している。
 * 
 * 最後に入力Nの三角形の内部に対して、テーブル値が0より大きい個数を求めればよい。
 * 
 * 計算量はO(M+N^2).
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
	// 右下方向へ
	for(i = 0; i <= N; i++)  // (y,x)=(i,1)スタートとして、(+1,+1)方向へ
	{
		for(j = 0; j < N; j++)  // diff
		{
			y = i+j;
			x = 1+j;
			if(y+1 > N || x+1 > N) continue;  // 範囲外
			imos_table[y+1][x+1] += imos_table[y][x];
		}
	}

	// 左下方向へ
	for(i = 0; i <= N; i++)  // (y,x)=(i,i+1)スタートとして、(+1,0)方向へ
	{
		for(j = 0; j < N; j++)  // diff
		{
			y = i  +j;
			x = i+1;
			if(y+1 > N) continue;  // 範囲外
			imos_table[y+1][x] += imos_table[y][x];
		}
	}

	// 右方向へ
	for(i = 1; i <= N; i++)  // (y,x)=(i,1)スタートとして、(0,+1)方向へ
	{
		for(j = 0; j < N; j++)  // diff
		{
			y = i;
			x = 1+j;
			if(y < x+1) continue;  // 三角形の範囲外
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
