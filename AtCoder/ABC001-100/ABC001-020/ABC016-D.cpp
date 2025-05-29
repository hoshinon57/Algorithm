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

// ABC016 https://atcoder.jp/contests/abc016

/*
 * ACはした…が、ちょっと不完全燃焼。
 * 理由は、本ソースコードのままではAOJのCGL_2_B(線分の交差判定)が通らないから。
 * 
 * 「2点を通る直線に対して、残りの2点がどちら側にあるか」の判定を用いる。
 * 2点を交代して同じ判定をし、ともに「別々の側にある」であれば交差するとみなせる。
 * 
 * 本問では、制約より線分上に他の点が存在しないのでACできる。
 * AOJのCGL_2_Bではそういった制約がないので通らない。
 *   https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_B&lang=ja
 * 
 * AOJでもACするには、point_positionの改良が必要っぽい。
 * 以下のccwを参考にする感じ。
 *   https://bakamono1357.hatenablog.com/entry/2020/04/29/025320
 *   https://sizk.hatenablog.com/entry/2020/03/25/150743
 * やるなら腰据えてライブラリ化が必要かな。
 * 
 * https://qiita.com/ykob/items/ab7f30c43a0ed52d16f2
 * のソースコードでも、本問題はACしたがAOJはダメだった。
**/

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

// 2つの線分(x0,y0)-(x1,y1) と (x2,y2)-(x3,y3) が交差するならtrue.
// point_positionが必要。
// [注意]点がもう片方の線分上にピッタリある場合の挙動は未確認。 ⇒ 正しく動作しないケースもある
bool isSegmentIntersecting(ll x0, ll y0, ll x1, ll y1, ll x2, ll y2, ll x3, ll y3)
{
	// 平行ならfalse.
	// 2線分が同一直線上にあるとpoint_position()の判定がうまくいかないため、それをはじく目的。
	// 平行だけならpoint_position()は動作するが、どうせ交差しないので合わせて除外してしまって良い。
	if((y1-y0)*(x3-x2) == (x1-x0)*(y3-y2)) return false;  // 積のオーバーフローには注意

	auto d1 = point_position(x0, y0, x1, y1, x2, y2);
	auto d2 = point_position(x0, y0, x1, y1, x3, y3);
	if(d1*d2 > 0) return false;

	d1 = point_position(x2, y2, x3, y3, x0, y0);
	d2 = point_position(x2, y2, x3, y3, x1, y1);
	if(d1*d2 > 0) return false;

	return true;
}

int main(void)
{
	ll i;
	ll ax, ay, bx, by; cin >> ax >> ay >> bx >> by;
	int N; cin >> N;
	vector<ll> x(N), y(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i];}

	ll cnt = 0;
	for(i = 0; i < N; i++)
	{
		// チョップとi個目の線分の交差判定
		if(isSegmentIntersecting(ax, ay, bx, by, x[i], y[i], x[(i+1)%N], y[(i+1)%N]) ) cnt++;
	}
	cout << cnt/2+1 << endl;

	return 0;
}
