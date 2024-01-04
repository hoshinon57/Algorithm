#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC206 https://atcoder.jp/contests/abc206

/*
 * 自力で解けず、解説を見た。
 *   https://atcoder.jp/contests/abc206/editorial/2101
 *   https://blog.hamayanhamayan.com/entry/2021/06/19/233919
 * 主客転倒で「最大公約数がgである組み合わせ」について数え上げる。
 * また「約数系包除原理」の考え方も必要。
 * 
 * cnt[i]を
 *   gcd(x,y)=i となる(x,y)の組み合わせの数
 * として定義する。
 * ただしx=gcd,y=gcdとなる組み合わせも許容してカウントする。
 * 
 * x,yはiの倍数なので、L<=x,y<=R におけるiの倍数の数をzとすると
 *   z=floor(R/i) - ceil(L/i) +1
 * となる。
 * このx,yの組み合わせはz*z個だが、gcdがi*2やi*3のケースも含まれてしまう。
 * そのためgcdが大きい方から計算していき、cnt[i*2],cnt[i*3],...を引いていったものがcnt[i]となる。
 * ※ABC162-E(Sum of gcd of Tuples (Hard))と似た考え方。
 * この計算は調和級数によりO(logN)でおさまる。
 * 
 * cnt[i]はこれで確定だが、解にはここからx=gcd,y=gcdの組み合わせを除外する必要がある。
 * これはL<=i<=Rのとき、z+z-1を引いてあげればよい。
 * ※(x,y)について(i,*)および(i,*)の組み合わせがz+z.
 *   (i,i)が重複しているので-1する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「gについて全探索する」という思考パターンを持つ。
 * ・約数の包除原理についての考え方。上から順に計算し、cnt[i*2],cnt[i*3],...を減算していく。
 * ・cnt[]にはいったん「x=g,y=g」という条件は無視しておいて、
 *   ansの計算時にのみ考慮してあげる、という思考パターンを持つ。
 */

int main(void)
{
	ll i, j;
	ll l, r; cin >> l >> r;
	ll ans = 0;
	ll MAX = 1000000;  // 10^6
	vector<ll> cnt(MAX+5);

	for(i = MAX; i > 1; i--)  // cnt[i]を求める
	{
		ll z = r/i - (l+i-1)/i + 1;
		ll zz = z*z;
		for(j = i*2; j <= MAX; j+=i)
		{
			zz -= cnt[j];
		}
		cnt[i] = zz;
		if(l<=i && i<=r)
		{
			zz -= z+z-1;
		}
		ans += zz;
	}
	cout << ans << endl;

	return 0;
}
