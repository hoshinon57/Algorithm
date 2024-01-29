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

// ABC133 https://atcoder.jp/contests/abc133

/*
 * i,jの取りうる範囲が広いので、探索範囲を狭めることを考える。
 * jを固定してiを増やしていったときに、ii=i+2019とすると
 *   i*j%2019 = ii*j%2019
 * となる。
 * よってiの探索幅は2019-1=2018個で十分で、[L,L+2019) の範囲で良い。
 * jの探索範囲も同様だが、こちらはi+1がスタートなので [i+1,i+1+2019) の範囲となる。
 */

int main(void)
{
	const ll MOD = 2019;
	ll i, j;
	ll l, r; cin >> l >> r;
	ll ans = MOD;
	for(i = l; i < min(l+MOD, r); i++)
	{
		for(j = i+1; j <= min(i+1+MOD, r); j++)
		{
			chmin(ans, i*j%MOD);
		}
	}
	cout << ans << endl;

	return 0;
}
