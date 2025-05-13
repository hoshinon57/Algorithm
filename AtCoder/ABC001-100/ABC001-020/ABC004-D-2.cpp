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

// ABC004 https://atcoder.jp/contests/abc004

/*
 * 以下の解説を元に解いた版。
 *   https://kmjp.hatenablog.jp/entry/2014/02/16/1030
 * 計算量は3乗ではなくO(N)とかになる。
 * 
 * ある色についてだけ考えたとき、初期位置を中心に左右に置くのが最適。
 * よってGの置く範囲について全探索し、
 * Rは初期位置を中心に置ければそれが最善。Gの範囲と被るならその分ずらす。
 * Bも同様。
**/

// 位置baseにnum個あり、[l,l+num)に移すときの操作回数を返す
ll calc(ll base, ll num, ll l)
{
	ll ret = 0;
	// 初期値baseと移動先[l,l+num)との位置関係で3択
	if(base < l)
	{
		ret = num*(num+1)/2;
		ret += (l-base-1)*num;
	}
	else if(base >= l+num)
	{
		ret = num*(num+1)/2;
		ret += (base-(l+num))*num;
	}
	else
	{
		ll n;
		// 左側
		n = base-l;  // 個数
		// 0+1+2+...+n
		ret += (n+1)*n/2;

		// 右側
		n = l+num-base-1;
		ret += (n+1)*n/2;
	}
	return ret;
}

int main(void)
{
	ll lr, lg, lb;
	ll r, g, b; cin >> r >> g >> b;
	ll ans = INF64;

	for(lg = -350; lg <= 50; lg++)  // G左端  [lg,lg+g)
	{
		ll cg, cr, cb;
		cg = calc(0, g, lg);

		// R
		// 左端は-100-(r/2)が最適 ただしlg-r以下
		lr = min(-100-r/2, lg-r);
		cr = calc(-100, r, lr);

		// B
		// 左端は100-(b/2)が最適 ただしlg+g以上
		lb = max(100-b/2, lg+g);
		cb = calc(100, b, lb);

		chmin(ans, cg+cr+cb);
	}
	cout << ans << endl;

	return 0;
}
