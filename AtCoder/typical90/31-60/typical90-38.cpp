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

// 競プロ典型90問:38 https://atcoder.jp/contests/typical90/tasks/typical90_al

/*
 * 解説の通り。
 *   https://twitter.com/e869120/status/1392612322410057729
 *   https://drken1215.hatenablog.com/entry/2023/11/21/020301
 * 
 * 求めたい値は A*B/gcd(A,B).
 * ただし計算途中でオーバーフローする可能性がある。
 * 
 * そのため
 *   A*B/g > 10^18
 *   B > 10^18/(A/g)  の右辺切り捨て
 * が同値であることを利用してLarge判定を行う。
 * 参考：https://noshi91.hatenablog.com/entry/2021/01/10/041543
 */

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
	ll a, b; cin >> a >> b;
	ll g = gcd(a, b);
	ll ag = a/g;
	if(b > 1000000000000000000/ag)
	{
		cout << "Large" << endl;
	}
	else
	{
		cout << ag*b << endl;
	}

	return 0;
}
