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

// ABC167 https://atcoder.jp/contests/abc167

/*
 * まず、同じ色で塗られている組kが0のときだけ考えると、
 *   M*(M-1)*(M-1)*...*(M-1) = M*(M-1)^(N-1)
 * となる。(そりゃそうだ)
 * 
 * 同じ色で塗られている組がkのときを考える。
 * このとき、左右で同じ色のブロックをひとかたまりで見ると、
 *   M*(M-1)*(M-1*)...*(M-1) = M*(M-1)^(N-k-1)
 * の色の選び方がある。これをAとする。
 * 次に、「(N-k)個のブロックを、k個ダブらせる」と考えてみる。
 * この通り数は「(N-k-1)個の仕切りとk個の要素を並べる」の重複組み合わせ的な考え方になり、
 *   (N-k-1+k)C(k) = (N-1)C(k)
 * となる。これをBとする。
 * 
 * すると、A*Bがkのときの答。
 * これをk=0～Kまでそれぞれ計算し、その和が答となる。
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

const ll NCK_SIZE = 200001;  // N<=2*10^5 など、問題文の制約に合わせる
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

int main(void)
{
	ll k;
	ll N, M, K; cin >> N >> M >> K;
	nCk_init();

	ll ans = 0;
	for(k = 0; k <= K; k++)
	{
		ll A = M * modpow(M-1, N-k-1, MOD);
		A %= MOD;
		ll B = nCk(N-1, k);
		ans += A*B;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
