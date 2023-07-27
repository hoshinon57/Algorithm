#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC184 https://atcoder.jp/contests/abc184

/*
 * 確率DP / メモ化再帰で解く。
 * func()のコメントおよび実装を参照。
 */

int A, B, C;
double memo[101][101][101];

// 現在、硬貨が順にa,b,c枚あったときに、操作終了(いずれか100枚になる)までの操作回数の期待値を返す
// メモ化再帰を行う
double func(int a, int b, int c)
{
	if(a == 100 || b == 100 || c == 100) return 0;  // 操作終了
	if(memo[a][b][c] > -0.5) return memo[a][b][c];  // メモ済 double型なので念のため幅を持たせて判定する

	// 例えば金貨を選んだ場合について、func(a+1,b,c)+1が操作回数となる
	// 金貨を選ぶ確率は a/(a+b+c) となる
	// これを各硬貨について計算する
	double d1, d2, d3;
	d1 = (func(a+1, b, c) + 1) * ((double)a / (a+b+c));
	d2 = (func(a, b+1, c) + 1) * ((double)b / (a+b+c));
	d3 = (func(a, b, c+1) + 1) * ((double)c / (a+b+c));

	memo[a][b][c] = d1+d2+d3;
	return memo[a][b][c];
}

int main(void)
{
	int i, j, k;
	cin >> A >> B >> C;
	for(i = 0; i <= 100; i++)
	{
		for(j = 0; j <= 100; j++)
		{
			for(k = 0; k <= 100; k++)
			{
				memo[i][j][k] = -1;  // メモ未
			}	
		}	
	}	

	cout << std::fixed << std::setprecision(10);
	cout << func(A, B, C) << endl;

	return 0;
}
