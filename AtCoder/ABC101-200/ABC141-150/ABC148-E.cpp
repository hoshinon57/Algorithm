#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC148 https://atcoder.jp/contests/abc148

/*
 * ※ある数Xにて末尾に続く0の個数は、min(Xを2で割り切れる回数, Xを5で割り切れる回数)となる。
 *   公式解説より。
 * 
 * まず、Nが奇数のときは「2で割り切れる回数」が常に0のため、答は0.
 * 以下はNが偶数のときについて考える。
 * 
 * (1) (2,4,6,...N)のうち、5で割り切れるものの個数
 * (2) (2,4,6,...N)のうち、5^2で割り切れるものの個数
 * (3) (2,4,6,...N)のうち、5^3で割り切れるものの個数
 * (x) …
 * の総和が答となる。
 * (1)の個数はN/5/2となる。/2は偶数しか無いため。
 * (2)の個数はN/5/5/2となる。
 * (3)以降も同様。
 * よってNを5で割りながら、N/5/2を答に加算していけばよい。
 */

int main(void)
{
	ll N; cin >> N;
	if(N% 2 != 0)
	{
		cout << 0 << endl;
		return 0;
	}
	
	ll ans = 0;
	while(N > 0)
	{
		ans += (N/5) / 2;
		N /= 5;
	}
	cout << ans << endl;

	return 0;
}
