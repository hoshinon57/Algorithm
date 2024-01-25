#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC224 https://atcoder.jp/contests/abc224

/*
 * 解説を参考に、主客転倒で解く。
 *   https://twitter.com/kyopro_friends/status/1451909185050853389
 *   https://atcoder.jp/contests/abc224/editorial/2805
 * 
 * N=|S|とする。
 * S=1234を例に、各桁の数字が何回足されるかを考える。
 * 1:1の位になるケース  1+2?3?4 で2^2*10^0
 *   10の位になるケース 1 2+3?4 で2^1*10^1
 *   100の位           1 2 3+4 で2^0*10^2
 *   1000の位          1 2 3 4 で2^0*10^3
 * 2:1の位になるケース  1?2+3?4 で2^2*10^0
 *   10の位            1?2 3+4 で2^1*10^1
 *   100の位           1?2 3 4 で2^1*10^2
 *   1000の位 は無い
 * 3:1の位になるケース  1?2?3+4 で2^2*10^0
 *   10の位            1?2?3 4 で2^2*10^1
 *   100,1000の位 は無い
 * 4:1の位になるケース  1?2?3?4 で2^3*10^0
 * 
 * よって上からi番目の数値について、
 *   (初項2^(N-2), 公比5, 項数N-1-i の等比数列の和) + 2^i*10^(N-1-i)
 * が足される回数となる。
 * これに数値をかけたものを、全ての桁について加算していけば答となる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・各文字ごとに考えてみる。
 * ・総和を求めるので、S=1234について「1についての総和」「2についての総和」を個別に足していっても同じ答になる。
 *   各文字が1の位,10の位,100の位,... となるのは何パターンかを高速に計算できれば求められそう。
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

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

int main(void)
{
	int i;
	string s; cin >> s;
	ll N = s.size();
	ll ans = 0;
	ll inv5_1 = modinv(5-1, MOD);
	for(i = 0; i < N; i++)
	{
		ll d = s[i] - '0';
		ll a = modpow(2, N-2, MOD);  // 初項
		ll n = N-1-i;  // 項数
		ll tmp = a * (modpow(5, n, MOD) + MOD - 1) % MOD;  // a(5^n-1)
		tmp = tmp * inv5_1 % MOD;  // r-1 で割る
		tmp += modpow(2, i, MOD) * modpow(10, N-1-i, MOD) % MOD;  // 端数的な 2^i*10^(N-1-i)
		tmp *= d;
		tmp %= MOD;

		ans += tmp;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
