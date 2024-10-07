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

// ARC174 https://atcoder.jp/contests/arc174

/*
 * まず、★3以下のレビューを追加しても無意味。
 * ★4をk4件、★5をk5件追加したときに、平均点を3以上にするには
 *   (A1+2*A2+3*A3+4*(A4+k4)+5*(A5+k5)) / (ΣA+k4+k5) >= 3
 *   ⇒ A1+2*A2+3*A3+4*(A4+k4)+5*(A5+k5) >= 3*(ΣA+k4+k5)
 *   ⇒ -2*A1-A2+(A4+k4)+2*(A5+k5) >= 0
 *   ⇒ k4+2*k5 >= 2*A1+A2-A4-2*A5
 * となり、最後の式の右辺は定数であることに着目すると、k4,k5を調整して最小費用で上記式を達成したい。
 * 
 * ここで、★4の2票と★5の1票は同価値、つまり可換であることが分かる。
 * (1)2*P4>=P5 つまり★4の方が"コスパ悪い"とき、
 *   ★4の2票は★5の1票に置き換えてしまえばよいので、★4の追加範囲は [0,2) となる。
 * (2)逆に★5の方がコスパ悪いとき、
 *   ★5の1票は★4の2票に置き換えてしまえばよいので、★5の追加範囲は [0,1) となる。(つまり追加しないのが最善)
 * 
 * (1)(2)をコスパの良しあしで片方に絞って探索してもよいが、面倒なら上記全パターン計算しても良い。
 * 以下の実装ではそうしている。
 * 
 * [典型]コスパの異なる2要素を組み合わせるとき、可換に着目して探索範囲を制限できるという考え方。
 */

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

void solve(void)
{
	int i;
	vector<ll> a(5+1); for(i = 1; i <= 5; i++) {cin >> a[i];}
	vector<ll> p(5+1); for(i = 1; i <= 5; i++) {cin >> p[i];}
	ll c = 2*a[1] + a[2] - a[4] - 2*a[5];  // これが0以上になれば良い
	if(c <= 0)
	{
		cout << 0 << endl;
		return;
	}

	ll ans = INF64;
	ll k4, k5;
	// all ★4
	k4 = c;
	k5 = 0;
	chmin(ans, k4*p[4]+k5*p[5]);

	// all ★5
	k4 = 0;
	k5 = ceil_div(c, 2LL);
	chmin(ans, k4*p[4]+k5*p[5]);

	// ★4 1票 残り★5
	k4 = 1;
	k5 = ceil_div(c-1, 2LL);
	chmin(ans, k4*p[4]+k5*p[5]);

	cout << ans << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}

	return 0;
}
