#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC266 https://atcoder.jp/contests/abc266

/*
 * 公式解説を元に実装した。
 * https://atcoder.jp/contests/abc266/editorial/4659
 * 
 * ベクトルの外積について参考にしたサイトは以下。
 * https://todai-counseling.com/?p=663
 * https://ngroku.com/?p=5086
 * 
 * 点aから点bを、原点に対して反時計回りに見ることを考える。
 * ベクトルa=(ax, ay, 0)とベクトルb=(bx, by, 0) の外積のZ成分を返す関数を作成すると、以下が言える。
 * ・戻り値が正；
 *   外積のZ成分が正、つまりaからbへ反時計回りに見た角度が180度未満
 * ・戻り値が負：
 *   外積のZ成分が負、つまりaからbへ反時計回りに見た角度が180度より大きい
 *   ※戻り値が負ということは、右ねじの法則よりベクトルa,bがなす角は時計回りになり、
 *     この角度は平行四辺形を作るため180度未満。
 *     よって反時計回りに見ると180度より大きくなる。
 * 
 * よって四角形の4つの角についてそれぞれ外積を計算し(反時計回りに見ることに注意)、
 * それらが全て正であれば凸四角形と判定できる。
 */

/*
 * ベクトルa=(ax, ay, 0)とベクトルb=(bx, by, 0) の外積のZ成分を返す
 * このとき以下が言える。
 * ・戻り値が正；
 *   外積のZ成分が正、つまりaからbへ反時計回りに見た角度が180度未満
 * ・戻り値が負：
 *   外積のZ成分が負、つまりaからbへ反時計回りに見た角度が180度より大きい
 */
int cross_product(int ax, int ay, int bx, int by)
{
	return ax*by-ay*bx;
}

int main(void)
{
	int ax, ay, bx, by, cx, cy, dx, dy;
	cin >> ax >> ay >> bx >> by >> cx >> cy >> dx >> dy;

	bool ok;
	if(cross_product(bx-ax, by-ay, dx-ax, dy-ay) > 0  // 角A(∠BAD)
	&& cross_product(cx-bx, cy-by, ax-bx, ay-by) > 0  // 角B(∠CBA)
	&& cross_product(dx-cx, dy-cy, bx-cx, by-cy) > 0  // 角C(∠DCB)
	&& cross_product(ax-dx, ay-dy, cx-dx, cy-dy) > 0) // 角D(∠ADC)
	{
		ok = true;
	}
	else
	{
		ok = false;
	}

	if(ok)
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
