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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC357 https://atcoder.jp/contests/abc357

/*
 * 等比数列の和。各種解説の通り。
 *   https://www.youtube.com/watch?v=yR-51b-0i4E
 *   https://atcoder.jp/contests/abc357/editorial/10183
 * 
 * Nの桁数をxとすると、
 *   初項:N
 *   項数:N
 *   公比:10^x (=r)
 * の等比数列の和が答となる。
 * (1-r^N)/(1-r)*N を求める。
 * 
 * 初回提出時、計算の最後にて ans=ans * N % MOD としており
 * Nが大きいときにオーバーフローしており1WA.
 * そろそろmodintを活用 or 自作すべきか。
 */

// n(>=0)の桁数を返す dig(0)=1
int cal_digit(long long n)
{
	if(n == 0) return 1;
	int c = 0;
	while(n > 0) {
		c++;
		n /= 10;
	}
	return c;
}

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
	ll N; cin >> N;
	ll d = cal_digit(N);
	ll r = modpow(10, d, MOD);  // 公比

	ll ans;
	ans = (modpow(r, N, MOD) - 1 + MOD) % MOD;  // 1-r^N
	ans = ans * modinv((r-1+MOD)%MOD, MOD) % MOD;  // 1-r で割る
	ans = ans * (N%MOD) % MOD;  // 初項Nをかける
	cout << ans << endl;

	return 0;
}
