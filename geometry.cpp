#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

/*
 * 幾何に関するまとめやライブラリ
 * ・2点が同一か isSamePoint
 * ・(bx,by)を基点としたときの(tx,ty)の偏角 angleTo
 * ・(bx,by)を基点として(tx,ty)を回転 rotatePoint
 * ・直線に対して点がどちらの位置にあるか point_position
 * ・ベクトルa,bの外積のZ成分 cross_product
 * ・3点が同一直線上にあるか IsColliniar
 * ・3点の外心の位置 circumcenter
 * 
 * [関連する問題]
 * ABC151-F
 * ABC207-D
 * ABC266-C
**/

// 2点(x1,y1),(x2,y2)が同一ならtrue
// 同一かどうかは軸ごとにeps未満か否かで判定する
bool isSamePoint(double x1, double y1, double x2, double y2, double eps_)
{
	return (abs(x1-x2) < eps_ && abs(y1-y2) < eps_);
}

// (bx,by)を基点としたときの、(tx,ty)の偏角を返す。
// イメージとして、(tx,ty)が右側にあれば0, 上側にあればπ/2(90度), 下側にあれば(-π/2)(270度).
// 戻り値の単位はラジアンで、[-π, π]の範囲。
// #include <cmath> が必要
// [制約]2点が同一座標ではないこと(atan2にて定義域エラーとなることがある)。
double angleTo(double bx, double by, double tx, double ty)
{
	return atan2(ty-by, tx-bx);
}

// (bx,by)を基点として、(tx,ty)をangleだけ "反時計回りに" 回転させた位置を返す。
// isSamePoint()とangleTo()が必要。
// (bx,by)と(tx,ty)が同一座標のとき、(tx,ty)をそのまま返す。
// eps_:同一座標の判定に用いる
// angle:単位はラジアン
pair<double,double> rotatePoint(double bx, double by, double tx, double ty, double angle, double eps_)
{
	if(isSamePoint(bx, by, tx, ty, eps_)) return {tx, ty};
	// (bx,by)基点に移動させ、angleだけ追加で回転し、移動を元に戻す
	tx -= bx;
	ty -= by;
	auto d = sqrtl(tx*tx + ty*ty);
	double ang = angleTo(0, 0, tx, ty) + angle;
	tx = cos(ang)*d + bx;
	ty = sin(ang)*d + by;
	return {tx, ty};
}

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
// [外心とは]
//   各頂点からの距離が等しく、外接円の中心である
//   各辺の垂直二等分線の交点である
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
		const double eps = 1e-5;
		assert( isSamePoint(0, 0, 1e-9, -(1e-9), eps));  // 誤差がEPS未満
		assert(!isSamePoint(0, 0, 1e-3,       0, eps));  // 誤差がEPSより大
		assert(!isSamePoint(0, 0,    0,    1e-3, eps));  // 誤差がEPSより大
		assert( isSamePoint(1000, 2000, 1000+1e-9, 2000-(1e-9), eps));  // 基点が原点以外
		assert(!isSamePoint(1000, 2000, 1000+1e-3,        2000, eps));
		assert(!isSamePoint(1000, 2000, 1000,        2000+1e-3, eps));
	}
	{
		const double eps = 1e-5;
		assert(abs(angleTo(0, 0,    1,     0) - 0.0) < eps);
		assert(abs(angleTo(0, 0,    0,    10) - M_PI_2) < eps);
		assert(abs(angleTo(0, 0, -100,     0) - M_PI) < eps);
		assert(abs(angleTo(0, 0,    0, -1000) - (-M_PI_2)) < eps);  // (3/2)π=-(1/2)π 戻り値の範囲は[-π, π]

		// 基点が原点
		assert(abs(angleTo(0, 0, 10, 50) - 1.373400766) < eps);
		assert(abs(angleTo(0, 0, 50, 10) - 0.197395560) < eps);
		assert(abs(angleTo(0, 0, -70, 20) - 2.863292994) < eps);
		assert(abs(angleTo(0, 0, 70, -20) - (-0.278299659)) < eps);
		// 基点を移動
		assert(abs(angleTo(10+0, -500+0, 10+10, -500+50) - 1.373400766) < eps);
		assert(abs(angleTo(10+0, -500+0, 10+50, -500+10) - 0.197395560) < eps);
		assert(abs(angleTo(10+0, -500+0, 10+(-70), -500+20) - 2.863292994) < eps);
		assert(abs(angleTo(10+0, -500+0, 10+70, -500+(-20)) - (-0.278299659)) < eps);
	}
	{
		const double eps = 1e-5;
		double bx = 0.0, by = 0.0;
		auto xy = rotatePoint(bx+0, by+0, bx+10, by+20, 1.0471975511966, eps);  // 60度回転
		assert(isSamePoint(bx+(-12.32050807), by+18.66025404, xy.first, xy.second, eps));
		// 基点を変更
		bx = -20; by = -30;
		xy = rotatePoint(bx+0, by+0, bx+10, by+20, 1.0471975511966, eps);  // 60度回転
		assert(isSamePoint(bx+(-12.32050807), by+18.66025404, xy.first, xy.second, eps));

		bx = by = 0.0;
		xy = rotatePoint(bx+0, by+0, bx+20, by+0, -1.5707963267949, eps);  // -90度回転
		assert(isSamePoint(bx+0, by+(-20), xy.first, xy.second, eps));
		// 基点を変更
		bx = 1000; by = -10000;
		xy = rotatePoint(bx+0, by+0, bx+20, by+0, -1.5707963267949, eps);  // -90度回転
		assert(isSamePoint(bx+0, by+(-20), xy.first, xy.second, eps));

		// 同一座標
		xy = rotatePoint(10+eps/100, 100+eps/100, 10, 100, M_PI, eps);
		assert(isSamePoint(10, 100, xy.first, xy.second, eps));
	}
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

