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

// ABC197 https://atcoder.jp/contests/abc197

/*
 * 幾何問題。
 * 
 * ・N角形の中心を求める
 * ・中心を原点に平行移動させるイメージで、x0を平行移動
 * ・x0の角度をatan2()で求める これをaとする
 * ・a+(2PI/N)がx1の角度
 * の手順で求められる。
 */

int main(void)
{
	int N; cin >> N;
	double x0, y0, xN2, yN2; cin >> x0 >> y0 >> xN2 >> yN2;
	double cx, cy;  // 中心
	cx = (x0+xN2)/2; cy = (y0+yN2)/2;
	double d = sqrt((x0-cx)*(x0-cx) + (y0-cy)*(y0-cy));  // 中心と各頂点の距離
	double a = atan2((y0-cy), (x0-cx));  // x0の角度
	a += 2*M_PI/N;  // x1の角度

	double x1, y1;
	x1 = cos(a) * d + cx;  // x1の角度に距離をかけて、平行移動させていたのを元に戻す
	y1 = sin(a) * d + cy;
	cout << std::fixed << std::setprecision(10);
	cout << x1 << " " << y1 << endl;

	return 0;
}
