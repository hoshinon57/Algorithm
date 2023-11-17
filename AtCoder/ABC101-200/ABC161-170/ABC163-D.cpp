#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC163 https://atcoder.jp/contests/abc163

/*
 * 10^100はものすごく大きな値なので、
 * 「x個選んだときの個数」と「x+1個選んだときの個数」が一致することは無い。
 * よって異なる個数を選ぶ場合は独立して考えられる。
 * 
 * x個選ぶときの「和としてあり得るものの個数」を考えると、
 *   和の最小値：0+1+2+...+(x-1) = x(x-1)/2
 *   和の最大値：(N-x+1)+(N-x+2)+...+N = (2N-x+1)*K/2
 * の間ならどの値も和として作ることができる。
 * 選んだ値を1つずつずらせば良いため。
 * よって 最大値-最小値+1 が求める個数となる。
 * 
 * 以上をK～N+1まで求めた総和が答。
 * 
 * [ACまでの思考の流れ]
 * ・10^100をどう見るか。
 *   この値そのものは計算する必要は無さそう。
 * ・「和としてあり得るものの個数」なので、和そのものを求める必要はない。
 * ・0,1,2,...,N からx個選ぶとき、その和としてあり得るものの個数は、
 *   (和の最大値)-(和の最小値)+1 で求められる。
 *   選んだ値を1つずつずらせば良いため。
 */

int main(void)
{
	ll i;
	ll N, K; cin >> N >> K;
	ll ans = 0;
	for(i = K; i <= N+1; i++)  // i個選ぶ
	{
		ll mi = i*(i-1)/2;
		ll ma = (2*N-i+1)*i/2;
		ans += ma-mi+1;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
