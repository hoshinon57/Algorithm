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

// ABC196 https://atcoder.jp/contests/abc196

/*
 * 関数fは ＿／￣ こんな形になる。
 *   https://twitter.com/kyopro_friends/status/1373269973276061697
 * 
 * ／ の左下を(x1,y1), 右上を(x2,y2)とする。(傾きは常に1であることに注意)
 * すると各操作は以下のように置き換えられる。
 *   t=1:y1,y2を+a
 *   t=2:
 *     a<y1 なら何もしない
 *     y1<=a<=y2 ならy1=aとなり、x1もその分ずれる。
 *     y2<a なら ￣ となり、y1,y2ともにaとなる。(※)
 *   t=3:t=2と同じ考え方。
 * (※)このとき、x1,x2は傾きが1になるような適当な値にする必要がある。
 * 
 * その後、クエリxに対してはx1,x2との大小関係でO(1)で回答できる。
 */

int main(void)
{
	ll x1, y1, x2, y2;
	x1 = y1 = -INF64;
	x2 = y2 = INF64;
	int i;
	int N; cin >> N;
	for(i = 0; i < N; i++)
	{
		ll a, t; cin >> a >> t;
		if(t == 1)
		{
			y1 += a;
			y2 += a;
		}
		else if(t == 2)
		{
			if(a < y1) {;}
			else if(y1 <= a && a <= y2)
			{
				x1 += a - y1;
				y1 = a;
			}
			else
			{
				y1 = y2 = a;
				x1 = x2 = 0;
			}
		}
		else
		{
			if(a < y1)
			{
				y1 = y2 = a;
				x1 = x2 = 0;
			}
			else if(y1 <= a && a <= y2)
			{
				x2 -= y2 - a;
				y2 = a;
			}
			else {;}
		}
	}
	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll x; cin >> x;
		ll ans = 0;
		if(x < x1) ans = y1;
		else if(x1 <= x && x <= x2)
		{
			ans = y1 + (x - x1);
		}
		else ans = y2;
		cout << ans << endl;
	}

	return 0;
}
