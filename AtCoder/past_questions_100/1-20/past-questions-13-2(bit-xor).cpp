#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 100000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題13
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2008yo/tasks/joi2008yo_e

/*
 * bit全探索で解く。
 * 
 * 考え方は past-questions-13-2.cpp と同じ。
 * 改善点として、列ごとに枚数を数えていく際に、元はforでカウントしたのに対し、
 * こちらではbit演算でO(1)でカウントしている。
 * 参考：https://atug.tokyo/?p=215 (この記事ではbitsetを使っている)
 * 
 * 計算量はO(2^R*C).
 * AtCoder上の実行時間は、O(2^R*CR)の約600msに対して約50msと、1/10以上に短縮できた。
 */

int main(void)
{
	int i, j;
	int R, C;
	cin >> R >> C;
	vector<int> a(C);  // a[i]:i列目の煎餅の状態を、1枚ごとbitで表現する(1行目が0bit目)
	for(i = 0; i < R; i++)
	{
		for(j = 0; j < C; j++)
		{
			int tmp;
			cin >> tmp;
			a[j] |= (tmp << i);  // 1行目が0bit目、2行目が1bit目
		}
	}

	int answer = 0;
	// 各行についてひっくり返す/ひっくり返さないの全組み合わせをbit全探索する
	for(i = 0; i < (1<<R); i++)
	{
		int sum = 0;
		// j列目についてそれぞれ、1～R行までのR個の煎餅のうち出荷できる枚数をカウントしていく
		for(j = 0; j < C; j++)
		{
			// 初期値が裏、かつ行をひっくり返さないなら出荷OK.
			// 初期値が表、かつ行をひっくり返すなら、こちらも出荷OK.
			// よって、iとa[j]のxorを取ったときに0のbitの数が出荷OKの枚数となる。
			// 
			// ただし使われないbitの0と混ざってしまっているので、
			// xor結果が1のbit, つまり出荷NGの枚数をカウントする。
			// するとR-countが出荷OKの枚数になる。
			// 列単位でもひっくり返せるため、R-count or R-(R-count) のうち大きい方を採用すればよい。
			// なので結局は count or R-count のうち大きい方。
			int count = 0;
			count = __builtin_popcount(i ^ a[j]);  // 出荷NGとなる枚数
			sum += max(count, R-count);
		}
		answer = max(answer, sum);
	}
	cout << answer << endl;

	return 0;
}
