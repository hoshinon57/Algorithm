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
 * 以下のユーザー解説を元に実装した。
 * https://atcoder.jp/contests/abc266/editorial/4723
 * 
 * 凸四角形ならば、点A,Cを結ぶ直線に対して点B,Dが異なる側にある。
 * (点B,Dを結ぶ直線側も同様)
 * 
 * よって直線に対する点の位置関係を返す関数func()を作成し、
 * 点B,Dを引数にしたときの戻り値の符号を確認すればよい。
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
double func(double x0, double y0, double x1, double y1, double target_x, double target_y)
{
	return (target_y - y0) * (x1 - x0) - (target_x - x0) * (y1 - y0);
}

int main(void)
{
	double ax, ay, bx, by, cx, cy, dx, dy;
	cin >> ax >> ay >> bx >> by >> cx >> cy >> dx >> dy;

	// 点A,Cを通る直線に対して、点B,Dの位置を判定する
	double ret_b = func(ax, ay, cx, cy, bx, by);
	double ret_d = func(ax, ay, cx, cy, dx, dy);

	// 点B,Dを通る直線に対して、点A,Cの位置を判定する
	double ret_a = func(bx, by, dx, dy, ax, ay);
	double ret_c = func(bx, by, dx, dy, cx, cy);

	bool ok = true;
	// 点A,Cを通る直線に対して点B,Dの位置が、
	//   同じ側にある：ret_b,ret_dの符号は同じ(共に正か、共に負)
	//   別の側にある：ret_b,ret_dの符号は異なる
	// よって積が正なら符号は同じと判定できる
	if(ret_b * ret_d > 0)
	{
		ok = false;  // 点B,Dが同じ側にあるため、凸四角形ではない
	}
	if(ret_a * ret_c > 0)
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
