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

// ABC165 https://atcoder.jp/contests/abc165

/*
 * 机上計算やテストコードをしてみると、どうやらx=B-1のときに最大になりそう、と分かる。
 * ただしNがB-1より小さいときはx=Nまで。
 * 
 * ※解説を見ると、xを変化させたときに結果は周期Bになっているので、
 *   x=[0,B-1]で考えるとA*floor(x/B)は常に0になることからも計算できる。なるほど…
 */

void check(ll a, ll b, ll n)
{
	ll x;
	for(x = 0; x <= n; x++)
	{
		ll tmp = a*x/b - a*(x/b);
		cout << "x=" << x << ":" << tmp << endl;
	}
}

int main(void)
{
	ll a, b, n; cin >> a >> b >> n;
	// check(a, b, n);
	ll x = min(b-1, n);
	cout << a*x/b << endl;

	return 0;
}
