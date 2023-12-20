#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC191 https://atcoder.jp/contests/abc191

/*
 * 自力で解けず、解説を見た。
 * (方針が分かっても誤差でWAを出したが…)
 * 
 * まず、入力は10000倍して整数にしておく。
 * 10000の倍数となる値が、元のスケールにて整数となる。
 * 
 * ya=k*10^4となるようなyaを全探索し、Y=yaとなる直線上の格子点を求めていく。
 * X方向の幅xdは、三平方の定理より xd*xd=(y-ya)*(y-ya)+r*r で求められる。
 * llroundしたときに誤差が出たら嫌なので、xdが想定より大きかったら-1する処理を入れている。(必要かどうかは不明)
 * 
 * 後はx-xdからx+xdの範囲で、k*10^4となるX座標の個数を求めて答に加算する。
 */

const ll mul = 10000;
void input(ll &x, ll &y, ll &r)
{
	double xd, yd, rd; cin >> xd >> yd >> rd;
//	x = (ll)(xd*mul+0.5);  // llに変換時に0の方向に切り捨てられるので、負のケースでWAになる
//	y = (ll)(yd*mul+0.5);
//	r = (ll)(rd*mul+0.5);
	x = llround(xd*mul);
	y = llround(yd*mul);
	r = llround(rd*mul);
}

int main(void)
{
	ll x, y, r;
	input(x, y, r);

	ll ans = 0;
	ll ya;  // ya=k*10^4 となるようなya,つまり元のスケールにて整数となるyaについて全探索 
	// yaのループ先頭、つまり最小のyを求める
	// 除算は0方向に切り捨てられるので、y-rの正負で処理が異なる
	if(y-r >= 0)
	{
		ya = ((y-r+(mul-1))/mul)*mul;  // 切り上げ
	}
	else
	{
		ya = ((y-r)/mul)*mul;
	}
	for(; ya <= y+r; ya+=mul)
	{
		ll tmp = r*r - (y-ya)*(y-ya);
		ll xd = llround(sqrt(tmp));
		if(xd*xd > tmp) xd--;  // 誤差の可能性があるので、判定して-1しておく

		// k*10^4, つまり元のスケールにて整数となるxの左右端を求める
		// startは右方向に、endは左方向に寄せる
		// 先ほどと同じく、正負で処理を分ける必要がある
		ll xst, xed;
		if(x-xd >= 0) xst = (x-xd+(mul-1))/mul;
		else xst = (x-xd)/mul;
		if(x+xd >= 0) xed = (x+xd)/mul;
		else xed = (x+xd-(mul-1))/mul;

		ans += xed - xst + 1;
	}
	cout << ans << endl;

	return 0;
}
