#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC188 https://atcoder.jp/contests/abc188

/*
 * 自力で解けず、解説チラ見した。
 *   https://x.com/kyopro_friends/status/1348264891057102851
 *   https://atcoder.jp/contests/abc188/editorial/471
 *   https://blog.tiramister.net/posts/abc188-f/
 * 
 * 操作を逆方向で考えて、
 *   +1, -1, /2
 * を用いてY->Xへ書き換えることを考える。
 * ※逆方向、の思いつき方が分からなかったが、/2は*2と違って偶数という制約が増えることで
 *   状態遷移が減らせる…という観点かなと。
 * 
 * func(n)を、n->Xへの操作回数を返す関数とする。
 * まずn<=Xなら+1を繰り返すしかないため、X-nを返す。いわゆるベースケース。
 * 
 * 色々と紙に書いて考えてみると、
 *   +1,+1,/2 はムダ。/2して+1した方が早いので。
 *   同様に-1,-1,/2 もムダ。
 * となる。
 * よって/2する前の+1,-1操作は1回まで。
 * 
 * 以上より以下の遷移が求められる。
 * nが偶数のとき：
 *   /2操作をする再帰か、-1を繰り返すかの2択。
 * nが奇数のとき：
 *   +1または-1操作をする再帰か、-1を繰り返すかの3択。
 *   前2択は、再帰先で/2をする前提。
 *   ※+1して/2, -1して/2, どちらが適切かはたぶんその時点では分からない気がする。
 *     よって両方向見ておく。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・操作を逆方向に見ることで、状態遷移を減らせるという考え方。
 * ・紙に色々書いて試す。
 *   本問であれば、+1および-1を2回以上やるのはムダっぽい、というのが分かる。
 */

ll X, Y;
map<ll,ll> cnt;  // cnt[n]:n->Xへの回数

// n->Xへの操作回数を返す
ll func(ll n)
{
	if(cnt.count(n)) return cnt[n];
	if(n <= X) return X-n;  // 手段は+1を繰り返すのみ（いわゆるベースケース）

	// 以下、n>X
	ll ret;
	if(n%2 == 0)
	{
		// /2操作か、-1を繰り返すか
		ret = min(1+func(n/2), n-X);
	}
	else
	{
		// +1(の後/2)か、-1(の後/2)か、最後まで-1を繰り返すか
		ret = min({1+func(n+1), 1+func(n-1), n-X});
	}
	return (cnt[n] = ret);
}

int main(void)
{
	cin >> X >> Y;
	cout << func(Y) << endl;

	return 0;
}
