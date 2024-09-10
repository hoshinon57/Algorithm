#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

/*
 * 幾何に関するまとめやライブラリ
 * ・直線に対して点がどちらの位置にあるか point_position
 * ・ベクトルa,bの外積のZ成分 cross_product
 * ・3点が同一直線上にあるか IsColliniar
 * ・3点の外心の位置 circumcenter
 * 
 * [関連する問題]
 * ABC151-F
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

// 3点が同一直線上にあるか調べる
// 3点A,B,CのA->Bの変化量を(dx1,dy1), A->Cの変化量を(dx2,dy2)とする
// 参考：https://qiita.com/tydesign/items/ab8a5ae52eb9c50ad26a
bool IsColliniar(int dx1, int dy1, int dx2, int dy2)
{
	// dy1/dx1 = dy2/dx2
	// となればよいので、dx1*dx2を両辺にかける
	return dx1*dy2 == dx2*dy1;
}

// 3点の外心の位置を返す
// [制約]
// (1)3点は同一線分上にないこと (呼び出し元でチェックすること)
// (2)|x|,|y|<=10^6程度 途中計算にて3乗まで出るため
//    それ以上の値を扱いたい場合、引数とローカル変数をdoubleに変更する形か(未検証)
// [参考]https://qiita.com/tatesuke/items/59133758ec25146766c3
pair<double,double> circumcenter(long long x1, long long y1, long long x2, long long y2, long long x3, long long y3)
{
	long long x1_2 = x1*x1;
	long long y1_2 = y1*y1;
	long long x2_2 = x2*x2;
	long long y2_2 = y2*y2;
	long long x3_2 = x3*x3;
	long long y3_2 = y3*y3;
	double y = (double)((x3-x1)*(x1_2+y1_2-x2_2-y2_2) - (x2-x1)*(x1_2+y1_2-x3_2-y3_2))
				/ (2*(x3-x1)*(y1-y2) - 2*(x2-x1)*(y1-y3));
	double x;
	if(x1 == x2) x = (double)(2*(y1-y3)*y - x1_2 - y1_2 + x3_2 + y3_2) / (2*(x3-x1));
	else         x = (double)(2*(y1-y2)*y - x1_2 - y1_2 + x2_2 + y2_2) / (2*(x2-x1));
	return {x, y};
}

int main(void)
{
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
	}

	{
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
	}

	/*
	外積を使うことで、
	・点が直線の左右どちらにあるか
	・点が三角形の内部にあるか
	などを簡単に判定することができる。
	https://imagingsolution.blog.fc2.com/blog-entry-51.html
	*/

	assert(IsColliniar(1, 2, 3, 6));
	assert(!IsColliniar(1, 2, 3, 5));
	assert(IsColliniar(1, 2, -5, -10));

	{
		auto circumcenter_test = []
		(long long x1, long long y1, long long x2, long long y2, long long x3, long long y3, double cx, double cy) -> void
		{
			const double EPS = 1e-8;
			auto d = circumcenter(x1, y1, x2, y2, x3, y3);
			// cout << std::fixed << std::setprecision(15);
			// cout << d.first << " " << d.second << endl;
			assert(abs(d.first-cx)  < EPS);
			assert(abs(d.second-cy) < EPS);
		};
		// https://keisan.casio.jp/exec/user/1511355526 を用いた
		circumcenter_test(0, 0, 1, 1, 0, 1, 0.5, 0.5);  // (0,0),(1,1),(0,1)を順繰りに3つ
		circumcenter_test(1, 1, 0, 1, 0, 0, 0.5, 0.5);
		circumcenter_test(0, 1, 0, 0, 1, 1, 0.5, 0.5);
		circumcenter_test(0, 0, -1, 0, 1, 1, -0.5, 1.5);
		circumcenter_test(5, 0, 1, 3, 1, 0, 3.0, 1.5);
		circumcenter_test(1000000, 999999, -500000, -999999, 12345, 54321, 2949515.1865181730, -2024638.4145270443);
	}

	return 0;
}

