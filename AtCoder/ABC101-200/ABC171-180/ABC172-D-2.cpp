#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC172 https://atcoder.jp/contests/abc172

/*
 * kyopro_friends氏のO(N)の解法を参考に実装した。
 * https://twitter.com/kyopro_friends/status/1276873988010995712
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	ll ans = 0;
	for(i = 1; i <= N; i++)
	{
		ll n = N/i;
		// 例えばi=3のとき、3,6,9,...,(Nを超えない最大)となるので、総和は
		// 3+6+9+...+(Nを超えない最大)
		// =3(1+2+3+...(N/3))
		// =3*(1+2+3+...+n)   (n=N/3とする)
		// =3*n*(n+1)/2
		// と、iからnまで公差1の等差数列になる。
		ans += i * n*(n+1)/2;
	}
	cout << ans << endl;


	return 0;
}
