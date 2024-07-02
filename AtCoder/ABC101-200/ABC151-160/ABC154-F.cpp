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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC154 https://atcoder.jp/contests/abc154

/*
 * 自力で解けず、解説を見た。
 *   https://img.atcoder.jp/abc154/editorial.pdf
 * 
 * 上記解説の通りで、表をイメージすると
 *   f(0,c)+f(1,c)+f(2,c)+...+f(r,c) = f(r,c+1)
 * が成り立つ。
 * 
 * c1<=c<=c2の各cについて、以下を求めたい。
 *   f(r1,c)+f(r1+1,c)+f(r1+2,c)+...+f(r2,c)
 * これは
 *   f(r2,c+1) - f(r1-1,c+1)
 * と式変形できるので、各cごとの和が答となる。
 * 
 * ※f(r,c)は二項係数にてC(r+c,r)で求められる。
 *   これは事前計算をすればO(1)で計算可能。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・二項係数は表をイメージし、「どことどこが一致するか」「どの区間の和がどこと一致するか」などを考えてみる。
 */

const ll MOD = 1e9+7;
const ll NCK_SIZE = 2000000+5;
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
	nCk_init();
	ll r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
	ll ans = 0;
	for(ll i = c1; i <= c2; i++)  // c=i
	{
		// (r,c)について、
		// f(0,c)～f(r2,c)の和はf(r2,c+1)と等しい -> C(r2+c+1, r2)
		// f(0,c)～f(r1-1,c)の和はf(r1-1,c+1)と等しい -> C(r1-1+c+1, r1-1)
		ans += nCk(r2+i+1, r2) - nCk(r1-1+i+1, r1-1) + MOD;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
