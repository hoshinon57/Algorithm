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

// ABC151 https://atcoder.jp/contests/abc151

/*
 * 主客転倒(というらしい)で解く。
 * 以下、0-indexedで記載する。
 * 
 * まず、数列Aを昇順ソートしておく。
 * AiがmaxXとなるのはどういうときか、何通りかを考えると、
 *   K個の中からまずAiを選び、
 *   残り(K-1)個を、A0～A(i-1)の中から選ぶ通り数
 * となる。
 * minXについても同様の考え方。
 * 
 * よって各A[i]について、
 *   maxXになる通り数 * A[i]  を解に加算
 *   minXになる通り数 * A[i]  を解から減算
 * していった結果が答となる。
 */

const ll MOD = 1e9+7;
const ll NCK_SIZE = 1e5+1;  // N<=2*10^5 など、問題文の制約に合わせる
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
	// 0-indexed
	int i;
	int N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end());

	nCk_init();
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		// Aiがmaxになるケース
		if(i >= K-1)
		{
			ans += nCk(i, K-1) * a[i] % MOD;
			ans %= MOD;
		}
		// Aiがminになるケース
		if(N-1-i >= K-1)
		{
			ans = ans + MOD - nCk(N-1-i, K-1) * a[i] % MOD;
			ans %= MOD;
		}
	}
	cout << ans << endl;

	return 0;
}
