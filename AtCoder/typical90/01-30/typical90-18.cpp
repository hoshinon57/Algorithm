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

// 競プロ典型90問:18 https://atcoder.jp/contests/typical90/tasks/typical90_r

/*
 * まずは時刻Eにおける観覧車の位置(ey,ez)を求めたい。
 * 角度はスタート時点で270度で、減算されていく形となる。
 * また半径はL/2, 中心は(Y,Z)=(0,L/2)となることから、ey,ezが求められる。
 * 
 * 俯角なので、像とのXY方向の距離とZ方向の距離を求めたい。
 *   XY方向の距離：(0,ey)と(X,Y)の距離
 *   Z方向の距離：ezと0の距離
 * から、atan2()で求められる。
 */

int main(void)
{
	ll T; cin >> T;
	ll L, X, Y; cin >> L >> X >> Y;
	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll e; cin >> e;
		double ang = 270.0 - ((double)e/T)*360.0;
		if(ang < 0.0) ang += 360.0;
		ang = ang*(M_PI*2)/360.0;  // ラジアン変換
		double ey, ez;
		// 観覧車の半径はL/2, 中心(Y,Z)=(0,L/2).
		ey = cos(ang)*L/2.0;
		ez = sin(ang)*L/2.0 + L/2.0;

		double exy = sqrt(X*X + (Y-ey)*(Y-ey));
		ang = atan2(ez, exy) * 360.0 / (M_PI*2);
		cout << std::fixed << std::setprecision(15);
		cout << ang << endl;
	}

	return 0;
}
