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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC356 https://atcoder.jp/contests/abc356

/*
 * bitごとに寄与を考える。evima氏の解説とほぼ同じ。
 *   https://www.youtube.com/watch?v=et42NbMEUZk
 * 
 * Nを0から増やしていったときに、それをbitごとに0,1を見ると周期的に変化している。
 * これは周期2^(i+1)であり、周期の先頭2^iが0, 後半2^iが1となっている。
 * 
 * よって各bitごとに周期をlとすると、周期部分とその端数でそれぞれ考えて、
 * 周期部分：
 *   N/lが周期の個数。これに2^iをかけた個数が答に寄与する。
 * 端数：
 *   N%lが端数の個数。
 *   先頭2^i個は0なので引き、0-indexedなので+1した個数が答に寄与する。下限0クリップに注意。
 * を計算していけばよい。
 */

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	ll ans = 0;
	for(i = 0; i < 60; i++)
	{
		if(((M>>i)&1) == 0) continue;
		ll p2 = (ll)1<<i;  // 2^i
		ll l = p2*2;  // 周期
		ll a, b;
		a = N/l * p2;  // 周期の個数 * 1周期あたりの1の個数
		b = N%l;  // 端数
		b = max(b-p2+1, (ll)0);  // 先頭p2個は0なので引く
		ans += a+b;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
