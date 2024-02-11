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

// ABC340 https://atcoder.jp/contests/abc340

/*
 * 解説を参考に、拡張ユークリッドの互除法にて解く。
 *   https://qiita.com/drken/items/b97ff231e43bce50199a  drkenさんのQiita記事が分かりやすい
 *   https://atcoder.jp/contests/abc340/editorial
 *   https://atcoder.jp/contests/abc340/submissions/50189964  YouTube解説の提出コード
 * 
 * ※本問の入力において、三角形の面積は以下。(ABC340-F.cppを参照)
 *   |AY-BX| = 2
 * 
 * ax+by=c が整数解を持つ条件：
 *   cがgcd(a,b)で割り切れること
 * であるので、まず 2 が gcd(a,b) で割り切れなければ-1を返して終了。
 * 
 * まずライブラリを用いて AY-BX=gcd(a,b) を満たすA,Bを求める。
 * そのA,Bに対して 2/gcd(a,b) をかけたものが答。
 */

long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

// 一次不定方程式 ax+by=gcd(a,b) を満たす {x,y} を格納する
// 戻り値：gcd(a,b)
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/b97ff231e43bce50199a
long long extgcd(long long a, long long b, long long &x, long long &y) {
	if (b == 0) { x = 1; y = 0; return a; }
	long long d = extgcd(b, a%b, y, x);
	y -= a/b * x;
	return d;
}

int main(void)
{
	ll x, y; cin >> x >> y;
	if(2 % gcd(x, y) != 0)
	{
		cout << -1 << endl;
		return 0;
	}
	ll a, b;
	ll g = extgcd(y, -x, a, b);  // AY-BX=2 を求めるので(かつX,Yが定数扱い)、引数の順番や"-x"に注意
	a *= 2/g;
	b *= 2/g;
	cout << a << " " << b << endl;

	return 0;
}
