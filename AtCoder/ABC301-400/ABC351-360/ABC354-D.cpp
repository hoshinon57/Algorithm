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

// ABC354 https://atcoder.jp/contests/abc354

/*
 * 二次元累積和の考え方で解く。
 * 横4,縦2のパターンが繰り返されていることを活用する。
 * 
 * まず、入力A～Dに適当な値を加算して、全て正の値にする。
 * ただしパターンからずれないよう4の倍数であること。
 * ⇒入力の最小値が-10^9のため、INF32近辺の値とした。
 * 
 * f(x,y)を、領域(0,0)～(x,y)に囲まれる長方形内の面積とすると、
 * f(C,D)-f(A,D)-f(C,B)+f(A,B) が答となる。
 * f(x,y)は、横4および縦2のループ部分と、その端数部分とで分けて計算すればよい。
 * 類題：ABC331-D(Tile Pattern)
 * 
 * [ACまでの思考の流れ]
 * ・パターン部分を見極める。
 *   面積はパターン部分とその端数部分でそれぞれ計算する。
 * ・二次元累積和の考え方を用いて、(0,0)起点に面積を求めるようにして実装を簡易にする。
 *   この場合負の値があると面倒なので、オフセットを足して正の値になるようにしておく。
 */

int main(void)
{
	ll a, b, c, d; cin >> a >> b >> c >> d;
	ll ofs = (INF32/4) * 4;  // -10^9に加算して正になり、かつ4の倍数
	a += ofs;
	b += ofs;
	c += ofs;
	d += ofs;

	// r[x][y]:(0,0)～(x,y)に囲まれる長方形内の、黒の面積
	ll r[4+1][2+1] = {
		{0, 0, 0},
		{0, 2, 3},
		{0, 3, 6},
		{0, 3, 7},
		{0, 4, 8},
	};
	// (0,0)～(x,y)に囲まれる長方形内の面積を返す
	auto func = [&](ll x, ll y) -> ll
	{
		ll lx = x/4;
		ll ly = y/2;
		ll zanx = x%4;
		ll zany = y%2;
		ll ret = 0;
		ret += lx * ly * r[4][2];  // 横4縦2のループ部分
		ret += r[4][zany] * lx;  // 横4のループ部分、縦は端数部分
		ret += r[zanx][2] * ly;
		ret += r[zanx][zany];
		return ret;
	};
	cout << func(c,d) - func(a,d) - func(c,b) + func(a,b) << endl;

	return 0;
}
