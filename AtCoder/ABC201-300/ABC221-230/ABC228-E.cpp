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
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC228 https://atcoder.jp/contests/abc228

/*
 * 自力で解けず、解説を見た。
 * 以下、p=998244353とする。
 * 
 * まず、本問題は M^(K^N) をpで割った余りが答となる。
 * ※ちなみに、M^(K^N) != M^K^N. 最初ミスった。
 *   例えば 2^(3^4) = 2^81,  2^3^4 = 2^12 で異なる値。
 * 
 * ただし a=(K^N)%p として M^a を求めるのは間違い。べき乗の部分は%pに置き換えられない。
 * 例：(M,K,N,MOD)=(3,3,3,5) にて確認すると分かる。
 * 
 * ここでフェルマーの小定理を使い、 M^(p-1) の形を作る。
 * K^Nをp-1で割った商をq, 余りをrとすると、 K^N=q(p-1)+r であり、
 * M^(q(p-1)+r)
 *  = M^(q(p-1)) * M^r
 *  = M^((p-1)q) * M^r
 *  ≡ 1^q * M^r = M^r (mod p)
 * となる。
 * このrは繰り返し二乗法を用い、modpow(K, N, MOD-1) で求められる。
 * 
 * あとはM^rをpで割った余りが答になるので、
 * 同じく繰り返し二乗法にて modpow(M, r, MOD) で求められる。
 * 
 * 最後にコーナーケースについて。
 * フェルマーの小定理 a^(p-1) ≡ 1 (mod p) は、pが素数、aがpの倍数ではないという条件がある。
 * 本問題の制約より、Mがpの倍数がありえるため(998244353*2, 998244353*3, ...)、
 * このときはコーナーケース扱いとする必要がある。
 * Mがpの倍数であれば、M^(K^N)%p は0になるので、0を出力して終わらせておく。
 */

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
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
	ll N, K, M; cin >> N >> K >> M;

	if(M % MOD == 0)  // コーナーケースを除外
	{
		cout << 0 << endl;
		return 0;
	}

	ll r = modpow(K, N, MOD-1);
	ll ans = modpow(M, r, MOD);
	cout << ans << endl;

	return 0;
}
