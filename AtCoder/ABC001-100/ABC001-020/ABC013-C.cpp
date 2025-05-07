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

// ABC013 https://atcoder.jp/contests/abc013

/*
 * 数学問題かな。
 * 
 * まず、食いだめをするのが良いというのは公式解説の通り。
 * 先に食事をして、後半は食事を抜くという形で考える。
 * 
 * 普通の食事をx日、質素な食事をy日、食事抜きをz日、となる。
 * 1つを固定して全探索してみる。[典型]
 * 
 * x+y=N-zより、
 * Bx+Dy+H-Ez > 0
 * これを整理すると x > (Ez+Dz-ND-H)/(B-D) となる。(ソースコード的には除算切り捨てして+1がxの最小値)
 * ⇒これ、xが負の場合は0に補正必要。
 *   また、x>N-zのケースはありえないのでスルー必要。
 * 
 * Ax+Cyのminを求めたくって、
 * Ax+C(N-z-x) より (A-C)x+C(N-z) の最小値を求めたい。
 * A-C>0, N-z>=0より、y切片が0以上で右上方向に進むグラフとなる。
 * ここに先ほどのxを代入すれば、zを決め打ちしたときの値が求まる。
 */

int main(void)
{
	ll N, H; cin >> N >> H;
	ll a, b, c, d, e; cin >> a >> b >> c >> d >> e;

	ll ans = INF64;
	for(ll z = 0; z <= N; z++)  // 食事を抜く日数zを固定
	{
		ll tmp1 = z*(e+d) - N*d - H;
		ll tmp2 = b-d;
		ll x;
		if(tmp1 < 0)
		{
			x = 0;  // 0に補正
		}
		else
		{
			x = tmp1/tmp2 + 1;
		}
		if(x > N-z) continue;  // ありえない
		ll tmp = (a-c)*x + c*(N-z);
		chmin(ans, tmp);
	}
	cout << ans << endl;

	return 0;
}
