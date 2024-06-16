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

// ABC358 https://atcoder.jp/contests/abc358

/*
 * コンテスト中に解けず、解説を見た。動的計画法で解く。
 *   https://x.com/kyopro_friends/status/1801977552397054333
 *   https://atcoder.jp/contests/abc358/editorial/10224
 * 
 * dp[i][j]を以下のように定義する。
 *   i種類目の文字までを使い、  (i=1～26)
 *   j文字の文字列を作るときの、  (j=0～K)
 *   文字列の作り方の総数
 * dp[26][1]～dp[26][K]の和が答。
 * 
 * DP遷移について。
 * 例えばdp[2][5]を求めるとき、ここで'B'を2文字使うとすると、
 * 既にある5-2=3文字に2文字を加えることになり、 dp[1][5-2]*Comb(5,2) を加算することになる。
 * Bをその他の数使うケースも同様に加算すればよいので、
 *   dp[1][5-k]*Comb(5,k)
 * の総和となる。
 * 一般化すると
 *   dp[i][j] = Σ(dp[i-1][j-k]*Comb(j,k))
 * となる。ここで 0<=k<=min(C[k],j) であることに注意。上限は入力のC[k]かjとなる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・コンテスト中は「2文字まで全パターン作り、そこに3文字目を作る」で考えており、
 *   それだと作った文字列が重複するので上手くいかなかった。
 *   ※ABにAを加えるケースとBAにAを加えるケース、どちらもABAがカウントされてしまう。
 * ・重複が発生しないような構築を考える必要がある。[典型]
 * ・重複するのは「同じ文字を、別タイミングで追加してしまうため」と考えると、
 *   同じ文字はある1つのタイミングで追加する、という方針が良さそう。
 *   ⇒DPに「x種類目の文字までを使い」といった形で持たせる[典型]
 */

const ll MOD = 998244353;
const ll NCK_SIZE = 1001;  // N<=2*10^5 など、問題文の制約に合わせる
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
	ll i, j, k;
	ll K; cin >> K;
	vector<ll> c(26+1); for(i = 1; i <= 26; i++) {cin >> c[i];}
	nCk_init();

	vector<vector<ll>> dp(26+1, vector<ll>(K+1, 0));  // dp[26+1][K+1]
	dp[0][0] = 1;
	for(i = 1; i <= 26; i++)  // dp[i][j]
	{
		for(j = 0; j <= K; j++)
		{
			for(k = 0; k <= min(c[i], j); k++)  // i種類目の文字をk文字使う
			{
				dp[i][j] += dp[i-1][j-k] * nCk(j, k);
				dp[i][j] %= MOD;
			}
		}
	}

	ll ans = 0;
	for(j = 1; j <= K; j++)
	{
		ans += dp[26][j];
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
