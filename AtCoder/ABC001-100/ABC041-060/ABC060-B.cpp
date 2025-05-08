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

// ABC060 https://atcoder.jp/contests/abc060

/*
 * 数学的に解いてみた。例題：ABC186-E(Throne)
 * 
 * Aの倍数を足したものは、整数kを使ってkAと書ける。
 * よって kA%B=C となるkが存在するかを判定すればよいので、
 *   kA≡C (mod B)
 * Aで割りたいので、A,Bが互いに素なら割れる(YES).
 * 互いに素でない場合でも、gcd(A,B,C)でA,B,Cそれぞれを割ってあげることで、A'とB'が互いに素になればOK.
 * そうでなければNO.
**/

// a,bの最大公約数を返す
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

int main(void)
{
	ll a, b, c; cin >> a >> b >> c;
	ll g = gcd(gcd(a, b), c);
	a /= g;
	b /= g;
	c /= g;
	if(gcd(a,b) == 1) cout << "YES" << endl;
	else cout << "NO" << endl;

	return 0;
}
