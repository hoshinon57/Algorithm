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
 * 二分探索を2回実施して解いた。
 * 以下、長方形の各辺を 左辺,右辺,上辺,下辺 と書く。
 * またA<=Bとする。
 * 
 * (未証明だが)三角形の3点は左辺、上辺、下辺にそれぞれ1点ずつ配置するのが最適っぽい。
 * 3点をそれぞれ P1:左辺の点(0,y1), P2:上辺の点(x2,A), P3:下辺の点(x3,0) と置く。
 * 
 * まずP1を決めうちした後、
 * P3が下辺の上に配置されるようにP2を配置したときに、
 * P2のx座標がB以下であればOK, Bより大きければNGとする。(P2が長方形内に入るか否かの観点)
 * P1のy座標を二分探索しつつ、さらにP2のx座標を二分探索した。
 * 
 * P1,P2が決まったときのP3の位置は、
 * P1を基準としてP2を60度時計回りに回転させた位置。
 * つまり度数法で-60=300度。
 * 
 * 数値は小数部もあるため、二分探索のループ回数は決め打ち。
 * 二分探索を2回やるため、それぞれ1000回とした。(1000^2=10^6)
 * 
 * P1のy座標の探索範囲は A/2<=y1<=A.
 * P2のx座標の探索範囲は 0<=x2<=2*A とした。
 * ⇒2*Aなのは、ある程度の長さがあればP1が(0,0)に配置されるケースとなり、それ以上は変わらないため。
 *   厳密には (2/√3)*A とかになる模様。
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

	double ans = 0.0;

	// P1:(0,y1)を固定したときの最適なP2(x2,A)を決めて、
	// x2<=Bならtrue, x2>Bならfalse
	auto cal = [&](double y1) -> bool
	{
		int k;
		double l, r;  // x2を二分探索
		l = 0.0;
		r = 2*a;  // これぐらい右側なら十分
		for(k = 0; k < 1000; k++)
		{
			double mid = (l+r)/2.0;  // P2:(mid,A)
			// P1を基準としてP2を-60度(=300度)回転させた点がP3.
			// P3のy座標が下辺より上か下かで判断する
			auto [x3, y3] = rot300(0, y1, mid, a);
			if(y3 >= 0.0) l = mid;
			else r = mid;
		}
		ans = dist(0, y1, l, a);  // 最後の答出力用
		return (l <= b);
	};

	double ok, ng;  // y1を二分探索
	ok = a/2.0;
	ng = a;
	for(i = 0; i < 1000; i++)
	{
		double mid = (ok+ng)/2.0;
		if(cal(mid)) ok = mid;
		else ng = mid;
	}

	cal(ok);  // ansへ設定
	cout << std::fixed << std::setprecision(15);
	cout << ans << endl;

	return 0;
}
