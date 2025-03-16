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

// ABC397 https://atcoder.jp/contests/abc397

/*
 * ABC397-D.cppに対し、二分探索で解いてみた版。
 * x^3-y^3を(x-y)=kとして整理した式
 *   y^2+k*y+(k^3-N)/3k
 * のグラフについてy>=0では単調増加になるため、二分探索が使える。
**/

int main(void)
{
	ll N; cin >> N;

	// ax^2+bx+c=0のうち、±の+側について解を返す
	// グラフの最小値がx<0にあり、x>=0については単調増加であることが前提 ⇒ 二分探索できる
	// 整数解が無ければ-1を返す
	auto cal = [](ll a, ll b, ll c) -> ll
	{
		ll l = 0;
		ll r = INF32;
		while(r - l > 1)
		{
			ll mid = (l+r)/2;
			if(a*mid*mid + b*mid + c <= 0) l = mid;
			else r = mid;
		}
		// lについて検算
		if(a*l*l + b*l + c == 0) return l;
		else return -1;
	};

	for(ll k = 1; k <= 1e6; k++)
	{
		if((k*k*k-N)%(3*k) != 0) continue;

		ll y = cal(1, k, (k*k*k-N)/(3*k));
		if(y == -1) continue;  // 整数解無し

		ll x = k+y;
		if(x <= 0 || y <= 0) continue;
		cout << x << " " << y << endl;
		return 0;
	}
	cout << -1 << endl;

	return 0;
}
