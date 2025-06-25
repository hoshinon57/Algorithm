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

// ABC385 https://atcoder.jp/contests/abc385

/*
 * 着手前に解説を見た。
 *   https://atcoder.jp/contests/abc385/editorial/11664
 *   https://junkobayashi.hatenablog.com/entry/2024/12/25/000011
 *   https://x.com/igeeeeee_/status/1872218256213414358
 * 
 * 方針は公式解説の通り。
 * まず何パターンか考えてみると、隣同士のビルについてのみ計算すればよい、となる。
 * ※3,4つのパターンを試すと、1つ飛びのビルで計算した値が答になるケースは無い。
 * 
 * がちゃがちゃと式変形をすると、
 *   -(h1-h2)/(x1-x2)*x1 + h1
 * がy切片になるのでこれを最大化すればよい。
 * しかし本問では許容誤差が10^(-9)と通常より小さく、上記式の "+h1以外の部分" と +h1 とを加算する際に、
 * 両者の絶対値に大きな差があると大きな誤差が発生する模様。(桁落ち？ or 丸め誤差？)
 * 
 * 「なるべく整数型で計算をし、最後に除算をする」という方針で式変形をすると
 *   (h2*x1-h1*x2)/(x1-x2)
 * となるので、これを使う。
 * ※答は正の値(入力例3に注意)になると思うので、負のときはスルーする。
 *   分子と分母をかけるとlong longでもオーバーフローしそうなので、
 *   分子と分母の符号が異なる、という判定にした。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・O(N^2)ではなく、実は隣同士の判定だけで済むことがある。
 * ・浮動小数点数の計算では、「なるべく整数型で計算をし、最後に除算をする」となるよう式変形を心掛ける。
 *   特に許容誤差が小さい場合。
**/

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> x(N), h(N); for(i = 0; i < N; i++) {cin >> x[i] >> h[i];}

	double ans = -100.0;
	bool flg = false;  // ansが更新されたらtrue
	for(i = 0; i < N-1; i++)  // ビルiとi+1を比較
	{
		ll tmp1 = h[i+1]*x[i] - h[i]*x[i+1];
		ll tmp2 = x[i]-x[i+1];
		// if(tmp1 == 0) continue;  // これはNG これがあると例3(答は0.000000000000000000)で-1出力になってしまう
		if( (tmp1 > 0 && tmp2 < 0) || (tmp1 < 0 && tmp2 > 0) ) continue;  // 負になるならスルー

		double a = (double)tmp1 / tmp2;
		chmax(ans, a);
		flg = true;
	}

	if(flg)
	{
		cout << std::fixed << std::setprecision(20);
		cout << ans << endl;
	}
	else
	{
		cout << -1 << endl;
	}

	return 0;
}
