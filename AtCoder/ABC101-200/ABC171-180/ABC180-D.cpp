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

// ABC180 https://atcoder.jp/contests/abc180

/*
 * 数学問題、いやー苦戦した…
 * 
 * Xがどんな値であっても、A倍と+Bのうち小さくなる方を採用する方針で良い。
 * また操作は A,A,A,...,A,B,B,... と前半がA倍、後半が+Bとなる。
 * よって"オーバーフローに注意しながら"、まずA倍操作、残りを+B操作していく。
 * 
 * まずはA倍操作について、せいぜい60回ほど実施すれば確実にYを超えるので、愚直にループを回す。
 * AX<Y 判定にてオーバーフローしないよう、X<Y/A で判定する。
 * 
 * +B操作は愚直にやるとTLEなので、まとめて処理する。
 * X+kB>=Y となる最小のkを求めたときに、k-1回が答となる。
 * k>=(Y-X)/B より、floor((Y-X)/B)で求まる。
 * 
 * ansをint型にしていて1WA.
 * 
 * [ACまでの思考の流れ]
 * ・まずは手で色々書いてみる。
 *   A倍と+Bのうち小さくなる方を採用するのは自明かな。
 *   たぶん、「まずはA倍とやれるだけ」「次に+Bをやれるだけ」という順番になりそう。
 * ・オーバーフローを防ぐ対応が大変…
 */

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

int main(void)
{
	ll x, y, a, b; cin >> x >> y >> a >> b;
	ll ans = 0;
	// A倍操作を可能な限り行う
	while(x <= (b+x)/a)  // ax<=b+x つまりA倍の方が適切
	{
		if(x < ceil_div(y,a))  // Yを超えない範囲で
		{
			x *= a;
			ans++;
		}
		else break;
	}
	// +Bはまとめて実施
	ans += ceil_div(y-x, b) - 1;
	cout << ans << endl;

	return 0;
}
