#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC292 https://atcoder.jp/contests/abc292

/*
 * 各種解説を参考に、二分探索を1回で済ます実装を行った。
 *   https://x.com/kyopro_friends/status/1632017240278331395
 *   https://strangerxxx.hateblo.jp/entry/20230307/1678162558
 *   https://atcoder.jp/contests/abc292/editorial/5884
 * 
 * 以下、長方形の各辺を 左辺,右辺,上辺,下辺 と書く。
 * またA<=Bとし、Bの上限を (2/√3)*A でクリップする。(それ以上は結果が変わらないため)
 * 
 * 三角形の3点のうち、1点は長方形の頂点と同じ位置になるのが最適。
 * 3点をそれぞれ以下のように配置する。
 *   P1:(0,0)   長方形の頂点と重ねる
 *   P2:(x2,A)  上辺に置く
 *   P3:(x3,y3) 右辺に置く
 * 
 * x2を 0<=x2<=(1/√3)*A の範囲で二分探索する。
 * x2を決め打ちしたときにP3の位置が決まるので、x3が長方形の内部にあるか否かを判定する。
 * これはx3がB以下かどうかで判定できる。
 * 二分探索を完了させると、そのときのP3は右辺の上に配置されており、ここが最適な位置。
 */

double dist(double x1, double y1, double x2, double y2)
{
	double d;
	d = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
	return sqrt(d);
}

// (x1,y1)を起点に、(x2,y2)を時計回りに300度回転させた点を返す
pair<double,double> rot300(double x1, double y1, double x2, double y2)
{
	/*
		(x2,y2)を(x1,y1)基準に移動
		(x2,y2)の角度tを求め、300度(=5/3PI)回転させる
		(cos(t), sin(t)) * dist  + (x1,y1) が求める点
	 */
	double d = dist(x1, y1, x2, y2);
	x2 -= x1;
	y2 -= y1;
	double t = atan2(y2, x2);
	t += M_PI * 5.0 / 3.0;  // 5/3PI = 300度
	double x, y;
	x = cos(t) * d + x1;
	y = sin(t) * d + y1;
	return {x, y};
}

int main(void)
{
	int i;
	double a, b; cin >> a >> b;
	if(a > b) swap(a, b);  // A<=Bとする
	chmin(b, 2/sqrt(3)*a);  // MAX (2/√3)*A

	auto cal = [&](double x2) -> bool
	{
		auto [x3,y3] = rot300(0, 0, x2, a);
		return (x3<=b);
	};

	double ok, ng;  // x2
	ok = 0.0;
	ng = 1/sqrt(3)*a;
	for(i = 0; i < 1000; i++)
	{
		double mid = (ok+ng)/2.0;
		if(cal(mid)) ok = mid;
		else ng = mid;
	}

	double ans = dist(0, 0, ok, a);  // P1とP2の距離
	cout << std::fixed << std::setprecision(15);
	cout << ans << endl;

	return 0;
}
