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
 * コンテスト中に解き切れなかった。
 * ABC186-E(Throne)と似た考え方で解く。
 * 
 * 与えられた3点の三角形の面積は |AY-BX|/2 となるので、
 *   |AY-BX| = 2
 * を解けばよい。
 * https://ouchimath.com/3point-menseki/
 * 以下、AY-BX=-2 で考える。 (後の計算が分かりやすかったので)
 * 
 * 式変形をしていくと、
 *   AY=-2+BX
 *   A=(-2+BX)/Y  (1)
 * より、このAを整数としたい。適当な整数kを使って
 *   -2+BX = kY
 *   -2+BX ≡ 0 (mod Y)
 *   BX ≡ 2 (mod Y)
 * より、XとYが互いに素であれば
 *   B ≡ 2/X (mod Y)
 * で求められる。
 * ここで、g=gcd(2,X,Y) とし、2,X,Yをgで割っておく。
 * その上でX,Yが互いに素でない場合、解は存在しない。
 * 互いに素なら、上記の式でBが求まる。
 * 
 * Aは(1)で求められる。
 * Aが整数になるような式変形をしてきたので、求めたBを代入すればAは整数になる(はず)。
 * 
 * X,Yが0のときは個別に処理が必要。
 * X=0のとき、
 *   AY = -2
 *   A = -2/Y
 * より、右辺が整数なら解あり、非整数なら解なしになる。
 * このときBは何でもよいので、0でも設定しておく。
 * Y=0の場合も同様。
 * 
 * [コンテスト時の反省点]
 * 公式解説では拡張ユークリッドだったが、
 * それを知らずとも ABC186-E(Throne) の応用でコンテスト中に解けそうなところまではできた。
 * ただX,Y=0のケースや、
 * 最後の計算で128bit整数を使うときに「符号なし」で計算してしまったなどがあり、WAが取り切れなかった。
 */

long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

void ng(void)
{
	cout << -1 <<endl;
	exit(0);
}

void out(ll a, ll b)
{
	cout << a << " " << b << endl;
	exit(0);
}

int main(void)
{
	ll x, y; cin >> x >> y;
	ll two = 2;
	ll g = gcd(two, gcd(x, y));
	x /= g;
	y /= g;
	two /= g;
	if(gcd(x, y) != 1) ng();

	ll a, b;
	if(x == 0)
	{
		if(-two%y != 0) ng();
		a = -two/y;
		b = 0;  // 何でもよいので0
		out(a, b);
	}
	if(y == 0)
	{
		if(two%x != 0) ng();
		a = 0;
		b = two / x;
		out(a, b);
	}
	b = two * modinv(x, y);
	b %= y;
	a = (ll)((-two + (__int128_t)b * x) / (__int128_t)y);  // 計算途中でlong longを超えることがあるので128bitで計算
	out(a, b);

	return 0;
}
