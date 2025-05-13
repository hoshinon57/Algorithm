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
 * 基本は全探索、の典型
 * 
 * 色ごと、まとまって置くのが最適だろう、となる。
 * 例えば緑5つを{-2,-1,0,3,4}と置くことはないだろう、と。
 * 
 * そこで3つの色それぞれ、どの範囲に置くかで全探索する。
 * 計算量はO(個数^3)的な。
 * 
 * 極端なケースを考えてみると、
 * Gは[-350,-350+G) および [50,50+G) の範囲だろう、となる。
 * 同様にRは [-650,-650+R) および [-100,-100+R) の範囲、
 * Bは [-200,-200+B) および [350,350+B) の範囲かなと。
 * もうちょい狭められると思うが、組み合わせは500^3とかなので、十分。
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
		ll cg = calc(0, g, lg);
		for(lr = -650; lr <= -100; lr++)  // [lr,lr+r)
		{
//			if(lr+r >= lg) continue;
			if(lr+r > lg) continue;
			ll cr = calc(-100, r, lr);
			for(lb = -200; lb <= 350; lb++)  // [lb,lb+b)
			{
//				if(lg+g >= lb) continue;
				if(lg+g > lb) continue;
				ll cb = calc(100, b, lb);
				chmin(ans, cg+cr+cb);
			}
		}
	}
	cout << ans << endl;

	return 0;
}
