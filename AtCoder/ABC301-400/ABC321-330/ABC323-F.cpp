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

// ABC323 https://atcoder.jp/contests/abc323

/*
 * 場合分けを丁寧にやる問題。
 * 自分は「高橋君を原点に移動」「荷物を第1象限へ移動」することとした。
 * 
 * ・荷物の隣に移動する
 * ・荷物をゴールへ動かす
 * の2フェーズに分けて考え、それぞれの移動回数を計算する。
 * 前者は「荷物を迂回する」、後者は「片方の軸の移動は不要」などのケースを場合分けする必要がある。
 */

int main(void)
{
	ll xa, ya, xb, yb; cin >> xa >> ya >> xb >> yb;
	ll xc, yc; cin >> xc >> yc;
	// A原点
	xb -= xa; yb -= ya;
	xc -= xa; yc -= ya;
	xa = ya = 0;
	// Bを第1象限へ
	if(xb < 0)
	{
		xa *= -1;
		xb *= -1;
		xc *= -1;
	}
	if(yb < 0)
	{
		ya *= -1;
		yb *= -1;
		yc *= -1;
	}

	// 高橋君が(tx,ty)まで移動する回数を返す
	auto move = [&](ll tx, ll ty) -> ll
	{
		ll add = 0;
		if(xa == xb && xb == tx)
		{
			if(ya < yb && yb < ty) add = 2;  // 迂回のため2歩余計に歩く必要がある
			if(ty < yb && yb < ya) add = 2;
		}
		if(ya == yb && yb == ty)
		{
			if(xa < xb && xb < tx) add = 2;
			if(tx < xb && xb < xa) add = 2;
		}
		return abs(tx-xa) + abs(ty-ya) + add;
	};
	// 高橋君が(sx,sy)にいて、その隣に荷物があるという条件で、ゴールまで届ける回数を返す
	auto push = [&](ll sx, ll sy) -> ll
	{
		ll dx = abs(xc-xb), dy = abs(yc-yb);
		ll add = 2;
		if(sx == xb && xb == xc) add = 0;  // Y方向の移動は不要のため、回り込む2歩は不要
		if(sy == yb && yb == yc) add = 0;
		return dx+dy+add;
	};

	ll ans = 0;
	ll dx = xc-xb, dy = yc-yb;
	ll tmp1, tmp2;
#if 1  // 場合分けを簡易にしてみた 考え方は#elseの方を参照
	ll ofsx, ofsy;
	ofsx = (dx>=0) ? -1 : 1;  // 荷物より右にゴールがある場合は、荷物の左側に移動する。左側にゴールがあれば右側に移動する
	ofsy = (dy>=0) ? -1 : 1;
	tmp1 = move(xb+ofsx, yb) + push(xb+ofsx, yb);
	tmp2 = move(xb, yb+ofsy) + push(xb, yb+ofsy);
#else
	if(dx >= 0 && dy >= 0)
	{
		// 荷物の下から動かすケースと、左から動かすケース
		tmp1 = move(xb-1, yb) + push(xb-1, yb);
		tmp2 = move(xb, yb-1) + push(xb, yb-1);
	}
	else if(dx >= 0 && dy < 0)
	{
		tmp1 = move(xb-1, yb) + push(xb-1, yb);
		tmp2 = move(xb, yb+1) + push(xb, yb+1);
	}
	else if(dx < 0 && dy >= 0)
	{
		tmp1 = move(xb+1, yb) + push(xb+1, yb);
		tmp2 = move(xb, yb-1) + push(xb, yb-1);
	}
	else
	{
		tmp1 = move(xb+1, yb) + push(xb+1, yb);
		tmp2 = move(xb, yb+1) + push(xb, yb+1);
	}
#endif
	ans = min(tmp1, tmp2);
	cout << ans << endl;

	return 0;
}
