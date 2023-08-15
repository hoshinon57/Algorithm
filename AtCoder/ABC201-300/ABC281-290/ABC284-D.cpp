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

// ABC284 https://atcoder.jp/contests/abc284

/*
 * まず、min(p,q)<=3√N(Nの3乗根)が成り立つ。
 * これが成り立たないと、p^2*q > Nとなり矛盾するため。
 * よって2から3√N(Nの3乗根)まで順に試し割りしていけば、この範囲で割り切れる値が必ず存在する。
 * ⇒実装では3*10^6までとした。
 * 
 * ある数xで割り切れたら、これはpかqのどちらかとなる。
 * 以下の手順で判定する。
 * (1)もう一度xで割り切れる場合
 *   p=x, q=N/x/x で終了。
 * (2)割り切れなかった場合
 *   q=x, p^2=N/xとなる。
 *   ここでp=sqrt(N/x)と安易にすると、誤差でWAとなる可能性があるため、(※1)
 *   llround(sqrt(N/x)) にて最も近い整数を返すようにする。(※2)
 *   
 * (※1)
 * 参考：
 *   https://zenn.dev/mod_poppo/articles/atcoder-beginner-contest-284-d
 *   https://qiita.com/mod_poppo/items/910b5fb9303baf864bf7
 * double型の精度は53bitのため、正しく表現できる整数は2^53まで。
 * x=2のときN/x=4.5*10^18≒2^62程度とのことで、double型では正確に表現できない可能性がある。
 * 
 * (※2)
 * 勉強として、上記Zennの記事を参考に二分探索でも求めてみた。
 * func()を参照。
 */

// xの平方根を二分探索で求めて返す (本問題専用の関数)
ll func(ll x)
{
	// xの最大は4.5*10^18 < 2^62
	ll l = 1;
	ll r = (ll)1<<31;
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(mid*mid <= x) l = mid;
		else r = mid;
	}
	return l;
}

int main(void)
{
	ll i;
	int T; cin >> T;
	while(T > 0)
	{
		ll N; cin >> N;
		ll p, q;
		for(i = 2; i <= 3000000; i++)
		{
			if(N%i != 0) continue;

			// iはpかq
			if((N/i)%i == 0)
			{
				p = i;
				q = N/i/i;
			}
			else
			{
				q = i;
				p = llround(sqrt(N/i));
				// p = func(N/i);
			}
			break;
		}
		cout << p << " " << q << endl;
		T--;
	}

	return 0;
}
