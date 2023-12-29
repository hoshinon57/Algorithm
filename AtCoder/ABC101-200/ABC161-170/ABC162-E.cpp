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

// ABC162 https://atcoder.jp/contests/abc162

/*
 * 主客転倒で考える。調和吸収の考え方も含む。
 * f(x)="gcd=Xとなるような数列の個数" として考えると、x=1～Kまでの f(x)*x の総和が答となる。
 *   https://img.atcoder.jp/abc162/editorial.pdf
 *   https://twitter.com/kyopro_friends/status/1249335824732155905
 *   https://blog.hamayanhamayan.com/entry/2020/04/12/225411
 * 
 * f(x)について考える。
 * K以下のxの倍数は y=K/x個 であり、数列としてはy^N通り作れそうに思える。
 * ただしx=2, N=3, K=8のケースにて、
 *   (4,4,8)はgcd=4
 *   (8,8,8)はgcd=8
 * のようにgcd!=xの数列も含んでしまう。
 * 
 * ならどうするか…というと、 f(2x),f(3x),f(4x),... のようにxの倍数の個数を引いてあげればよい。つまり
 *   f(x) = y^N - f(2x) - f(3x) - f(4x) - ... 
 * となる。
 * このためにはf(x)をxの大きい方から求めていく必要がある。
 * ※ここの計算量は調和級数によりO(logK)であり、間に合う
 * 
 * [ACまでの思考の流れ]
 * ・数え上げ問題は見方を変える。
 *   各数列についてGCDを求めるのではなく、GCD=xとなる数列の個数を求めるのはどうか。
 */

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	ll N, K; cin >> N >> K;
	ll ans = 0;
	vector<ll> cnt(K+5);  // cnt[i]:gcd=iになるような数列の数
	ll x, y, z;
	for(x = K; x >= 1; x--)  // gcd=xとなる数列
	{
		y = K/x;  // xの倍数の個数
		ll cnt_now = modpow(y, N, MOD);
		for(z = x*2; z <= K; z+=x)  // f(2x),f(3x),... を引いていく
		{
			cnt_now = cnt_now + MOD - cnt[z];
			cnt_now %= MOD;
		}
		ans += (cnt_now * x) % MOD;  // f(x)*xを加算
		ans %= MOD;
		cnt[x] = cnt_now;
	}
	cout << ans << endl;

	return 0;
}
