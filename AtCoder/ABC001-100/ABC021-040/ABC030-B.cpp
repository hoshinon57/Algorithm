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

// ABC030 https://atcoder.jp/contests/abc030

// まず、13時と1時は同じなので、nを12で割った余りにしておく。
// また、0時の方向を0度としてイメージする。
// 分の角度：60分で360度なので、6*m.
// 時の角度：12時間で360度、また60分で30度なので、30*n+m/60*30.
// ここから角度を求めて、「小さい方の角度」を答えるので180度を超えてたら補正。

int main(void)
{
	int n, m; cin >> n >> m;
	n %= 12;
	double deg_ji = 30*n + (double)m/60*30;
	double deg_fun = 6*m;

	double ans = abs(deg_ji - deg_fun);  // ans>=0
	if(ans > 180.0) ans = 360.0-ans;
	cout << std::fixed << std::setprecision(10);
	cout << ans << endl;

	return 0;
}
