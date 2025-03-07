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

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

// ABC063 https://atcoder.jp/contests/abc063

/*
 * 二分探索で解く。各種解説と同じ考え方。
 *   https://img.atcoder.jp/arc075/editorial.pdf
 *   https://blog.hamayanhamayan.com/entry/2017/06/03/231648
 * 
 * 「爆発X回でクリアできるか」を考える。
 * X回でクリアできるならX+1回でもクリアできるので、単調性がある。
 * よって判定問題が解ければ、二分探索で求められる。
 * 
 * 各魔物に対して、Aダメージを与える回数をna回、Bダメージをnb回とすると、na+nb=X.
 * Σna<=Xであれば、爆発X回でクリアできる。
 * A*na+B*nb>=h となる最小のnaを求めたくて、式を整理すると
 * na >= (h-X*B)/(A-b)
 * となる。
 * 右辺の除算は切り上げ。
 *   https://noshi91.hatenablog.com/entry/2021/01/10/041543
**/

int main(void)
{
	ll i;
	ll N, A, B; cin >> N >> A >> B;
	vector<ll> h(N); for(i = 0; i < N; i++) {cin >> h[i];}

	// 爆発x回でクリアできるならtrue
	auto check = [&](ll x) -> bool
	{
		ll cnt = 0;  // ダメージAを与えた回数
		for(i = 0; i < N; i++)
		{
			ll tmp1 = (h[i] - x*B);
			ll tmp2 = A-B;
			ll tmp3 = ceil_div(tmp1, tmp2);
			cnt += max(tmp3, 0LL);  // 下限0クリップ
		}
		return (cnt <= x);
	};

	ll ok = INF32;
	ll ng = 0;
	while(ok - ng > 1)
	{
		ll mid = (ok+ng)/2;
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
