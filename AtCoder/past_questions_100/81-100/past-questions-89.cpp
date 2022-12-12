#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題89
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2013ho/tasks/joi2013ho1

/*
 * 入力データに対して、"0 1 0..." の交互列が続いている数を数える。
 *   1 1 0 0 1 0 1 1 1 0
 * であれば、 1, 2, 4, 1, 2 となる。これを配列data[]に保持しておく。
 * 
 * 反転操作をどう使うのが有効か？ を考えると、
 *   1 0 1 0 1 0 
 * という入力であれば、操作をするのはムダになる。
 *   1 0 1 0 0 1 0 1
 * という入力であれば、交互列が切り替わる後ろ4つに対して操作するのが最善になりそう。
 * 
 * よって前述のdata[]について、交互列の単位で反転操作をした場合に、
 *   data[i-1]+data[i]+data[i+1]
 * の値が出来上がった交互列になる。
 * この最大値を求めればよい。
 * 
 * 実装では、まず入力に対して "0 1 0..." の交互列を求めてdata[]に設定する。
 * 次にdata[]の各要素について、1つ前と1つ後ろの要素との和を計算し、その最大値を出力する。
 * ※処理しやすいように、data[]の先頭と末尾に番兵を入れている
 * 
 * 計算量はO(N).
 */
int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<int> a(N);
	for(i = 0; i < N; i++) cin >> a[i];

	vector<int> data;  // "0 1 0..." が続いている要素数を入れていく
	data.push_back(0);  // 番兵
	int count = 0;
	int prev = -1;  // 今見ているa[i]の手前の値
	for(i = 0; i < N; i++)
	{
		if(prev != a[i])
		{
			// 前の値と異なるなら "0 1 0..." の交互列が続いているので、カウントを増加
			count++;
		}
		else
		{
			// 前の値と一致していたら "0 1 0..." の交互列ではないので、
			// そこまでのカウントを出力して、カウントリセット
			data.push_back(count);
			count = 1;
		}
		prev = a[i];
	}
	data.push_back(count);  // 最後の分のカウント
	data.push_back(0);  // 番兵

	// data[]単位で反転操作を行う
	// この場合、各data[i]について、[i-1]と[i+1]との3つの和が、出来上がる交互列の数になる
	int answer = 0;
	for(i = 1; i < (int)data.size()-1; i++)
	{
		answer = max(answer, data[i-1]+data[i]+data[i+1]);
	}
	cout << answer << endl;

	return 0;
}
