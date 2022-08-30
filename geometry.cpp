#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 幾何に関する関数やまとめ

/*
 * [関連する問題]
 * ABC266-C
 */

/*
 * 2点(x0,y0),(x1,y1)を通る直線に対して、点(target_x,target_y)がどちら側にあるかを正負で返す。
 * (target_x, target_y)を異なる2通りで関数呼び出しして、
 * 戻り値の符号が同じであれば2点は同じ側、符号が異なれば2点は異なる側にあると判定できる。
 * ※本関数は、x0=x1やy0=y1となる場合でも正しく判定できる。
 * 
 * [処理の解説]
 * 上記2点を通る直線の式は、
 *   (y-y0)=(y1-y0)/(x1-x0)*(x-x0)
 * より、式変形して以下となる。
 *   (y-y0)(x1-x0)-(x-x0)(y1-y0)=0
 * 参考：http://www5d.biglobe.ne.jp/~tomoya03/shtml/algorithm/Intersection.htm
 *       https://www.st-hakky-blog.com/entry/2018/09/05/012837
 * x1>x0のとき、この式の左辺に(x',y')を代入した結果が、
 *   正である：点(x',y')は直線の上側にある
 *   負である：点(x',y')は直線の下側にある
 * と判定できる。
 * (x1<x0のときは、上側下側が反対になる。左辺のyの符号が逆になるため)
 */
double point_position(double x0, double y0, double x1, double y1, double target_x, double target_y)
{
	return (target_y - y0) * (x1 - x0) - (target_x - x0) * (y1 - y0);
}

/*
 * ベクトルa=(ax, ay, 0)とベクトルb=(bx, by, 0) の外積のZ成分を返す
 * 点aから点bを、原点に対して反時計回りに見るときに、以下が言える。
 * ・戻り値が正；
 *   外積のZ成分が正、つまりaからbへ反時計回りに見た角度が180度未満
 * ・戻り値が負：
 *   外積のZ成分が負、つまりaからbへ反時計回りに見た角度が180度より大きい
 *   ※戻り値が負ということは、右ねじの法則よりベクトルa,bがなす角は時計回りになり、
 *     この角度は平行四辺形を作るため180度未満。
 *     よって反時計回りに見ると180度より大きくなる。
 * 
 * ベクトルの外積について参考にしたサイトは以下。
 * https://todai-counseling.com/?p=663
 * https://ngroku.com/?p=5086
 */
int cross_product(int ax, int ay, int bx, int by)
{
	return ax*by-ay*bx;
}

int main(void)
{
	double x0, y0, x1, y1;
	double tmp1, tmp2;
	x0 = 1;
	y0 = 1;
	x1 = 2;
	y1 = 4;
	// 2点(1,1),(2,4)を通る直線に対して、点(3,3)と(0,3)は異なる側にある。
	tmp1 = point_position(x0, y0, x1, y1, 3, 3);
	tmp2 = point_position(x0, y0, x1, y1, 0, 3);
	if(tmp1 * tmp2 < 0)  // 異なる側 -> 戻り値の符号が異なる -> 積は負
	{
		cout << "test:point_position" << endl;
	}

	int ax, ay, bx, by, cx, cy;
	// A(1,1),B(3,0),C(2,4)のとき、ベクトルABとACのなす角は180度未満。
	// ただしBからCへ反時計回りに見る。
	ax = 1; ay = 1;
	bx = 3; by = 0;
	cx = 2; cy = 4;
	if(cross_product(bx-ax, by-ay, cx-ax, cy-ay) > 0)
	{
		cout << "test:cross_product 1" << endl;
	}
	// A(1,1),B(3,0),C(2,-4)のとき、ベクトルABとACのなす角は180度より大きい。
	// ただしBからCへ反時計回りに見る。
	ax = 1; ay = 1;
	bx = 3; by = 0;
	cx = 2; cy = -4;  // 先ほどの例からcyのみ変更
	if(cross_product(bx-ax, by-ay, cx-ax, cy-ay) < 0)
	{
		cout << "test:cross_product 2" << endl;
	}

	/*
	外積を使うことで、
	・点が直線の左右どちらにあるか
	・点が三角形の内部にあるか
	などを簡単に判定することができる。
	https://imagingsolution.blog.fc2.com/blog-entry-51.html
	*/

	return 0;
}

