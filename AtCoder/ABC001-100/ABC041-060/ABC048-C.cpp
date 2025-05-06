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

// ABC048 https://atcoder.jp/contests/abc048

/*
 * 良問なGreedyだった。
 * 
 * まず要素a1,a2についてa1+a2>xのとき、
 * a1を操作してa1+a2<=xにしたとして、
 * a2を操作してa1+a2<=xにしても損しない。a2を減らすぶん、a2+a3にも効いてくるので。
 * 
 * よって先頭から2個ずつ見ていって、総和がxより大きいぶんだけ後ろ側を減らしていく。
 * ただし前側も減らさないと足りないケースがあるので、そこのケアは必要。
 * ⇒よく考えたら、この実装にて前側も減らす必要があるのは最初の2個だけかも。
 *   なので最初の2個だけ特出しで処理するか、
 *   もしくは全要素をx以下に前処理しておくのでも良さそう。
**/

int main(void)
{
	ll i;
	ll N, x; cin >> N >> x;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = 0;
	for(i = 0; i < N-1; i++)
	{
		ll tmp = a[i]+a[i+1];
		if(tmp > x)
		{
			ll rem = tmp-x;  // 減らす量
			if(a[i+1] >= rem)
			{
				a[i+1] -= rem;
				ans += rem;
			}
			else  // 前側も減らさないとダメ
			{
				rem -= a[i+1];
				ans += a[i+1];
				a[i+1] = 0;

				a[i] -= rem;
				ans += rem;
			}
		}
	}
	cout << ans << endl;

	return 0;
}
