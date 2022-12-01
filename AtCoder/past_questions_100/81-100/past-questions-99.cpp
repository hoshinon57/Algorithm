#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題99
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/ddcc2020-qual/tasks/ddcc2020_qual_d

/*
 * 「操作手順によらず予選ラウンド数は同じ」まではたどり着いたが、
 * その後の実装ができず、解説ACした。
 * 
 * 以下の解説の通りに実装しているので、そちらを参照。
 *   https://img.atcoder.jp/ddcc2020-qual/editorial.pdf
 *   https://drken1215.hatenablog.com/entry/2020/01/25/221900
 */

int main(void)
{
	int i;
	int M;
	cin >> M;
	ll digit = 0;  // 桁数
	ll d_sum = 0;  // 各桁の数値の和
	for(i = 0; i < M; i++)
	{
		ll d, c;
		cin >> d >> c;
		digit += c;
		d_sum += d*c;
	}

	cout << digit-1 + (d_sum-1)/9 << endl;

	return 0;
}
