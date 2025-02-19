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

// ABC046 https://atcoder.jp/contests/abc046

/*
 * 次の得票数をTに合わせるかAに合わせるかをそれぞれチェック。
 * 票数の和が少ない方を採用する。
 * 
 * 各種解説で、2パターンを計算してmaxを取るものが多かったが、意図が解釈できず…うーむ。
**/

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

int main(void)
{
	ll i;
	ll N; cin >> N;
	ll x=1, y=1;
	for(i = 0; i < N; i++)
	{
		ll t, a; cin >> t >> a;  // x->t, y->a
		// x->tに合わせるケース
		ll x2 = ceil_div(x, t)*t;
		ll y2 = x2/t*a;
		if(y2 < y) y2 = INF64;  // 票が減るケースを除外

		// y->aに合わせるケース
		ll y3 = ceil_div(y, a)*a;
		ll x3 = y3/a*t;
		if(x3 < x) x3 = INF64;

		if(x2+y2 < x3+y3)
		{
			x = x2;
			y = y2;
		}
		else
		{
			x = x3;
			y = y3;
		}
	}
	cout << x+y << endl;

	return 0;
}
