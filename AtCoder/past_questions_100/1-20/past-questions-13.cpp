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
 * 行ごとにひっくり返す/ひっくり返さないの全組み合わせを、bit全探索する。
 * 行は最大10と小さいためこの手法が使える。(2^10)
 * 
 * 次に各列について、1～R行までR個の煎餅のうち、出荷できる枚数をカウントしていく。
 * これは
 *   初期値が裏、かつ行をひっくり返さないなら出荷OK.
 *   初期値が表、かつ行をひっくり返すなら、こちらも出荷OK.
 * を1枚ずつ見ていく形になる。
 * 列単位でもひっくり返せるため、"カウント" or "行数-カウント"の大きい方を採用する。
 * 
 * 列ごとのカウントの総和、の最大値が答となる。
 * 
 * 計算量はO(2^R*CR).
 * R=10,C=10000のため10^8となるが、実行制限時間内に間に合う。
 */

int main(void)
{
	int i, j, k;
	int R, C;
	cin >> R >> C;
	vector<vector<int>> a(R, vector<int>(C));  // a[R][C]
	for(i = 0; i < R; i++)
	{
		for(j = 0; j < C; j++)
		{
			cin >> a[i][j];
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
			int count = 0;
			for(k = 0; k < R; k++)
			{
				// 初期値が裏、かつ行をひっくり返さないなら出荷OK.
				// 初期値が表、かつ行をひっくり返すなら、こちらも出荷OK.
				if(a[k][j] == 0 && ((i>>k)&1) == 0) count++;
				if(a[k][j] == 1 && ((i>>k)&1) == 1) count++;
			}
			// 列単位でもひっくり返せるので、
			// count or R-count のうち大きい方を採用する
			// （R=10で出荷OKが3枚なら、列単位でひっくり返して10-3=7枚を採用できる、という考え方）
			sum += max(count, R-count);
		}
		answer = max(answer, sum);
	}
	cout << answer << endl;

	return 0;
}
