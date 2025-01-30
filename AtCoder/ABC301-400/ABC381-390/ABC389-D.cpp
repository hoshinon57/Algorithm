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

// ABC389 https://atcoder.jp/contests/abc389

/*
 * 尺取り法で解く。基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc389/editorial/11932
 * 
 * 正方形の中心を(x,y)として、まずx=0またはy=0にある正方形を考える。
 * この条件にて円内にあるものは (R-1)*4+1 個であるため、まずこれをカウントしておく。
 * ※(R-1,0)を中心とする正方形が必ず円内にあるのか？ が最初気になったが、
 *   (R-1+0.5)^2+(0.5)^2 と R^2 の大小関係を調べると、R>=1において常にR^2の方が大きいため、
 *   必ず円内にある。
 * 
 * 次に第1象限にて、x=1～(R-1)の整数xについてカウントしていく。
 * これはxの大きい方から尺取り法で求められる。
 */

int main(void)
{
	ll R; cin >> R;
	ll ans = (R-1)*4+1;  // x=0またはy=0にある正方形の数を初期値
	R *= 10;  // 10倍して整数で完結させる

	// 正方形の中心が(x,y)のとき、それは円内か
	auto check = [&](ll x, ll y) -> bool
	{
		return (x+5)*(x+5) + (y+5)*(y+5) <= R*R;
	};

	ll y = 0;
	for(ll x = R-10; x >= 10; x-=10)
	{
		// 条件を満たす間、yを移動
		while(check(x, y+10)) y += 10;

		ans += y/10*4;  // 対称性により4倍
	}
	cout << ans << endl;

	return 0;
}
