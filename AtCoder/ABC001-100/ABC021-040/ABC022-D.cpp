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

// ABC022 https://atcoder.jp/contests/abc022

/*
 * 自力で解けず、WEB検索で「重心」のキーワードまでチラ見して解いた。
 * 公式解説が分かりやすい。
 * 
 * 不変量に着目するんだろうな～までは思い浮かんだが、具体的なものが思いつかず。
 * 
 * 平行移動と回転って、具体的な値を求めるのって難しくない？ となる。(特に移動)
 * とすると、
 *   平行移動と回転では変化しない値
 *   拡大で変化する値
 * を見つけたい気持ちになる。
 * 
 * 解説では凸包もあったが、本ソースコードは重心を用いるものとした。
 * 重心と各点との距離を一覧にしてソートすると、それは平行移動と回転では変化せず、拡大で変化する。
 * よってソートしたものの末尾要素の変化が答となる。
 * 
 * ※最初、先頭要素の変化で提出したらWA.
 *   先頭の方が小さいので除算で誤差が大きくなったのかな…とか思ったが、
 *   先頭の要素は"重心との距離=0"になるケースがありうる。そりゃダメだ。
 * 
 * ※ソートしてO(NlogN)実装だが、重心との最遠点だけで十分なので、O(N)でいける。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・操作での不変量に着目する。
 * ・平行移動や回転では、「重心との距離」は変化しない。
**/

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<double> ax(N), ay(N); for(i = 0; i < N; i++) {cin >> ax[i] >> ay[i];}
	vector<double> bx(N), by(N); for(i = 0; i < N; i++) {cin >> bx[i] >> by[i];}

	// 星の位置{x,y}から重心をもとめ、
	// 各星と重心との距離をリストアップ＆ソートして返す
	// ※先頭のコメントに書いた通り、最遠点を返す処理で十分だった
	auto cal = [&](vector<double> &x, vector<double> &y) -> vector<double>
	{
		double gx = 0, gy = 0;
		for(i = 0; i < N; i++)
		{
			gx += x[i];
			gy += y[i];
		}
		gx /= N;
		gy /= N;
		vector<double> ret(N);
		for(i = 0; i < N; i++)
		{
			double dx = x[i]-gx;
			double dy = y[i]-gy;
			ret[i] = sqrt(dx*dx+dy*dy);
		}
		sort(ret.begin(), ret.end());
		return ret;
	};
	auto da = cal(ax, ay);
	auto db = cal(bx, by);

	cout << std::fixed << std::setprecision(20);
//	cout << db[0]/da[0] << endl;  // これだと重心との距離=0のケースでNG
	cout << db.back() / da.back() << endl;

	return 0;
}
