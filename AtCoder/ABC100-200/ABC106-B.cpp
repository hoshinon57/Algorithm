#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題2より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc106/tasks/abc106_b

/*
 * 引数で渡された値の約数の数を返す関数を作り、それを1～Nまで呼び出せばよい。
 * 約数を求めるのは、引数Nに対して√Nまで順に試し割していく。
 * 参考：https://qiita.com/drken/items/a14e9af0ca2d857dad23
 * 
 * 計算量はO(N*√N).
 */

// nの約数の数を返す
int divisor_num(int n)
{
	int count = 0;
	for(int i = 1; i*i <= n; i++)  // √Nまで試し割り
	{
		if(n%i == 0)
		{
			count++;
			if(i*i != n) count++;  // 5*5=25のような場合を除き、n/iも約数
		}
	}

	return count;
}

int main(void)
{
	int N;
	cin >> N;

	int answer = 0;
	for(int i = 1; i <= N; i += 2)
	{
		if(divisor_num(i) == 8) answer++;
	}
	cout << answer << endl;

	return 0;
}
