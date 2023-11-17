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

// ABC156 https://atcoder.jp/contests/abc156

/*
 * 答は 2^n-1-nCa-nCb となる。
 * -1は「1本以上選ぶ」という制約より。
 * 
 * 2^nは繰り返し2乗法にて。
 * nCrは、nの最大が10^9と大きいので、
 *   nCr=(nPr)/(r!)
 * の式で求める。
 */

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

// nCr=nPr/r! より求める
ll nCr(ll n, ll r, ll mod)
{
	ll i;
	ll ans = 1;
	for(i = n-r+1; i <= n; i++)
	{
		ans = (ans*i) % mod;
	}
	ll tmp = 1;
	for(i = 1; i <= r; i++)
	{
		tmp = (tmp*i) % mod;
	}
	ans = ans * modinv(tmp, mod) % mod;

	return ans;
}

int main(void)
{
	ll n, a, b; cin >> n >> a >> b;
	ll ans = modpow(2, n, MOD);
	ans += MOD - nCr(n, a, MOD);
	ans += MOD - nCr(n, b, MOD);
	ans = ans + MOD - 1;
	ans %= MOD;
	cout << ans << endl;

	return 0;
}
