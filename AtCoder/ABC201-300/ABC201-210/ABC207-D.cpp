#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC207 https://atcoder.jp/contests/abc207

/*
 * 方針は立ったがWAが取れず。ChatGPTに方針を示してもらった。
 * 
 * まずN=1のときはYes. コーナーケースとして処理しておく。
 * 以下、N>=2.
 * 
 * 以下の方針で実装する。
 *   1)Sの点iと、Tの1個目の点が一致すると決め打ちする。
 *   2)操作によって点の重心も同じように移動することから、
 *     「Sの重心を基点とした点iの偏角」と「Tの重心を基点とした点1の偏角」の差より、回転量が求まる。
 *   3)ここから平行移動量も求まる。
 *   4)Sの各点に処理をし、Tと一致すればOK.
 * 
 * ※Tの1個目の点が重心にならないよう処理必要。
 *   (2)における偏角が求められないから。
 * 
 * しかしWAが取れず。
 * ChatGPTを用いて試行錯誤したところ、(4)の判定に問題があった。
 * 移動した座標をそのままsetなりvector(&sort)なりに登録すると、浮動小数点型の誤差で順序が不安定になることがあるらしい。
 * 対策として登録時に llround(x*1e6) として定数倍＆整数丸めすることで、ACした。
 * ハマりポイントっぽいのでしっかり覚えておく。
 * (逆に、EPSは1e-6でも1e-9でもACした)
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

bool solve(void)
{
	const double eps = 1e-6;
	const ll mul = 1e6;  // setに登録時の定数倍
	ll i, j;
	ll N; cin >> N;
	using pll = pair<ll,ll>;
	set<pll> tset;
	vector<ll> a(N), b(N); for(i = 0; i < N; i++) {cin >> a[i] >> b[i];}
	vector<ll> c(N), d(N); for(i = 0; i < N; i++) {cin >> c[i] >> d[i]; tset.insert({llround(c[i]*mul), llround(d[i]*mul)});}

	if(N==1) return true;

	double gsx=0, gsy=0, gtx=0, gty=0;  // 重心
	for(i = 0; i < N; i++)
	{
		gsx += a[i];
		gsy += b[i];
		gtx += c[i];
		gty += d[i];
	}
	gsx /= N;
	gsy /= N;
	gtx /= N;
	gty /= N;

	// Tの1個目の点が重心にならないよう外しておく
	if(isSamePoint(gtx, gty, c[0], d[0], eps))
	{
		swap(c[0], c[1]);
		swap(d[0], d[1]);
	}

	for(i = 0; i < N; i++)  // Sの点iと、Tの1個目の点が一致すると決め打ち
	{
		double ang;
		if(isSamePoint(gsx, gsy, a[i], b[i], eps))
		{
			ang = 0.0;
		}
		else
		{
			double d1 = angleTo(gtx, gty, c[0], d[0]);
			double d2 = angleTo(gsx, gsy, a[i], b[i]);
			ang = d1 - d2;
		}
		auto [nx, ny] = rotatePoint(0, 0, a[i], b[i], ang, eps);
		double dx = c[0] - nx;
		double dy = d[0] - ny;
		// 回転ang, 移動(dx,dy)

		set<pll> sset;
		for(j = 0; j < N; j++)
		{
			auto [xx, yy] = rotatePoint(0, 0, a[j], b[j], ang, eps);
			xx += dx;
			yy += dy;
			sset.insert({llround(xx*mul), llround(yy*mul)});
		}
		if(sset == tset) return true;
	}
	return false;
}

int main(void)
{
	YesNo(solve());
	return 0;
}
