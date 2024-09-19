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

// ABC151 https://atcoder.jp/contests/abc151

/*
 * 最小包含円をO(N^4)で解く。
 * 
 * どんな円が解になるかを考えると、
 *   (1)N個のうち3個を通る円
 *   (2)N個のうち2個を直径とする円
 * のどちらかになるんじゃないかな…となる。
 * (1)の組み合わせがN^3なので、各円の判定にO(N)かかると考えると、全体でO(N^4)となり間に合う。
 * 
 * まず(2)の方は簡単。
 * 2点から円の中心および半径が簡単に求められるので。
 * 
 * (1)については、3点が鋭角三角形をなす場合は外接円となる。
 * 外心の位置を求めればよく、以下を参考にした。
 *   https://qiita.com/tatesuke/items/59133758ec25146766c3
 * 実装はcircumcenter()を参照。
 * 
 * 3点が鈍角三角形をなす場合、外心を同じように求めると三角形の外側になる。
 * しかしこれよりも、最長の辺の中点を円の中心にする方が、小さな包含円となる。
 * ここの場合分けが厄介そうに思えるが(三角形の角度を求める必要があるので)、
 * 実は(2)で計算できているので、特に気にする必要は無い。
 * 
 * またN=2や全点が一直線上にあるケースでも(2)は使えないが、
 * (2)を計算することで最小包含円を求めることができる。
 * 
 * テクニックとして、求めた円でN点を全て含むかチェックする際に、10^(-8)程度を加算している。
 * 浮動小数点の誤差があるので、多少許容させる形。
 * 詳しくはcheck()の実装を参照。
 * 
 * [参考]
 * https://drken1215.hatenablog.com/entry/2020/01/12/224200
 * https://kmjp.hatenablog.jp/entry/2020/01/13/1030
 */

// 3点の外心の位置を返す
// [制約]
// (1)3点は同一線分上にないこと (呼び出し元でチェックすること)
// (2)|x|,|y|<=10^6程度 途中計算にて3乗まで出るため
//    それ以上の値を扱いたい場合、引数とローカル変数をdoubleに変更する形か(未検証)
// [参考]https://qiita.com/tatesuke/items/59133758ec25146766c3
pair<double,double> circumcenter(long long x1, long long y1, long long x2, long long y2, long long x3, long long y3)
{
	long long x1_2 = x1*x1;
	long long y1_2 = y1*y1;
	long long x2_2 = x2*x2;
	long long y2_2 = y2*y2;
	long long x3_2 = x3*x3;
	long long y3_2 = y3*y3;
	double y = (double)((x3-x1)*(x1_2+y1_2-x2_2-y2_2) - (x2-x1)*(x1_2+y1_2-x3_2-y3_2))
				/ (2*(x3-x1)*(y1-y2) - 2*(x2-x1)*(y1-y3));
	double x;
	if(x1 == x2) x = (double)(2*(y1-y3)*y - x1_2 - y1_2 + x3_2 + y3_2) / (2*(x3-x1));
	else         x = (double)(2*(y1-y2)*y - x1_2 - y1_2 + x2_2 + y2_2) / (2*(x2-x1));
	return {x, y};
}

// 3点が同一直線上にあるか調べる
// 3点A,B,CのA->Bの変化量を(dx1,dy1), A->Cの変化量を(dx2,dy2)とする
// 参考：https://qiita.com/tydesign/items/ab8a5ae52eb9c50ad26a
bool IsColliniar(int dx1, int dy1, int dx2, int dy2)
{
	// dy1/dx1 = dy2/dx2
	// となればよいので、dx1*dx2を両辺にかける
	return dx1*dy2 == dx2*dy1;
}

int main(void)
{
	const double EPS = 0.00000001;  // 10^(-8)
	int i, j, k;
	int N; cin >> N;
	vector<int> x(N), y(N);
	for(i = 0; i < N; i++) cin >> x[i] >> y[i];
	double ans = INF64;

	auto dist = [](double x1, double y1, double x2, double y2) -> double
	{
		double dx = x1-x2, dy = y1-y2;
		return sqrt(dx*dx+dy*dy);
	};

	// 中心が(px,py), 半径diの円が全ての点を囲むならtrue
	auto check = [&](double px, double py, double di) -> bool
	{
		int ii;
		for(ii = 0; ii < N; ii++)
		{
			if(dist(px, py, x[ii], y[ii]) < di+EPS) {;}
			else return false;
		}
		return true;
	};

	// 2点
	double px, py, di;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			px = (double)(x[i]+x[j]) / 2;
			py = (double)(y[i]+y[j]) / 2;
			di = dist(px, py, x[i], y[i]);
			if(check(px, py, di))
			{
				chmin(ans, di);
			}
		}
	}

	// 3点
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			for(k = j+1; k < N; k++)
			{
				if(IsColliniar(x[j]-x[i], y[j]-y[i], x[k]-x[i], y[k]-y[i])) continue;
				auto tmp = circumcenter(x[i], y[i], x[j], y[j], x[k], y[k]);
				px = tmp.first;
				py = tmp.second;
				di = dist(px, py, x[i], y[i]);
				if(check(px, py, di))
				{
					chmin(ans, di);
				}
			}
		}
	}

	cout << std::fixed << std::setprecision(15);
	cout << ans << endl;

	return 0;
}
