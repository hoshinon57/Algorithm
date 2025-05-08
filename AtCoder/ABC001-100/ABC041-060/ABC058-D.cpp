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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC058 https://atcoder.jp/contests/abc058

/*
 * 数え上げ、主客転倒の問題。drken氏の解説と同じ方針だった。
 *   https://drken1215.hatenablog.com/entry/2024/10/15/234417
 * 
 * 「和の形の数え上げ」は主客転倒に置き換えると解けるケースがありそう。の典型。
 * 
 * 隣同士の直線、つまりx[i],x[i+1]で挟まれた区間で考えたときに、それが全体で何回カウントされるかを考える。
 * まずx方向だけで考えると、n=5,x[1]とx[2](0-indexed)で挟まれた区間は、
 *   (1+1)*(5-2)
 * 回カウントされる。これは上記drken氏の解説に分かりやすい図がある。
 * 一般化すると (i+1)*(n-i-1).
 * 
 * 上記にxのdiffを乗算したものを加算しておいて(=xsum)、
 * y方向も同じようにカウントしたものをxsumとかけて、その総和が答。
 */

int main(void)
{
	ll i;
	ll n, m; cin >> n >> m;
	vector<ll> x(n); for(i = 0; i < n; i++) {cin >> x[i];}
	vector<ll> y(m); for(i = 0; i < m; i++) {cin >> y[i];}

	mint xsum;
	for(i = 0; i < n-1; i++)  // 全(n-1)個 i個目(0-indexed) 幅はx[i+1]-x[i]
	{
		mint tmp = (i+1)*(n-i-1);
		tmp *= x[i+1]-x[i];
		xsum += tmp;
	}

	mint ans;
	for(i = 0; i < m-1; i++)
	{
		mint tmp = (i+1)*(m-i-1);
		tmp *= y[i+1]-y[i];
		tmp *= xsum;
		ans += tmp;
	}
	cout << ans.val() << endl;

	return 0;
}
