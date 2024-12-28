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
using mint = modint998244353;

// ABC360 https://atcoder.jp/contests/abc360

/*
 * modintで解いてみた版。
 * dp遷移式を整理すると
 *   newdp = (N^2-2N)/(N^2)*dp + 2/(N^2)
 * となる。
 * 実装にて定数部分をそれぞれp,qと置いている。
 */

int main(void)
{
	ll i;
	ll N, K; cin >> N >> K;
	mint dp = 1;
	mint p = ((mint)N*N-2*N) / (N*N);
	mint q = (mint)2 / (N*N);
	for(i = 0; i < K; i++)
	{
		mint ndp;
		ndp = p*dp + q;
		swap(ndp, dp);
	}
	mint ans = dp + (1-dp)/2*(N+2);
	cout << ans.val() << endl;

	return 0;
}
