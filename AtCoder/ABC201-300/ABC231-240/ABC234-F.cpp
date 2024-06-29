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

// ABC234 https://atcoder.jp/contests/abc234

/*
 * 公式解説と同じ方針だった。嬉しい。
 *   https://atcoder.jp/contests/abc234/editorial/3223
 *   https://x.com/kyopro_friends/status/1479812647415840769
 * 
 * 文字を追加していくような問題にて、DPに「x種類目の文字までを使い」といった形で持たせてみる。 [典型]
 * ex:ABC358-E(Alphabet Tiles)
 * 
 * len=|S|とする。
 * dp[i][j]を以下のように定義する。
 *   i種類目の文字まで見て、 (i=1～26)
 *   全体でj文字の文字列を作るときの、 (j=0～len)
 *   文字列の種類数
 * 答はdp[26][*]の総和。(ただし空文字列はダメなので、*は1以上)
 * 
 * dp[i][j]への遷移は、i種類目の文字をk文字使うとして、dp[i-1][j-k]からの遷移となる。これは
 *   (i-1)種類目までを(j-k)文字
 *   i種類目をk文字
 * 並べる並べ方なので、C(j,k)通りとなる。
 * つまり各kについて dp[i-1][j-k]*C(j,k) の総和がdp[i][j]となる。
 */

const ll MOD = 998244353;
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
	int i, j, k;
	string s; cin >> s;
	ll len = s.size();
	vector<ll> cnt(26+1);
	for(auto &e : s) cnt[e-'a'+1]++;

	nCk_init();

	vector<vector<ll>> dp(26+1, vector<ll>(len+1, 0));  // dp[26+1][len+1]
	dp[0][0] = 1;
	for(i = 1; i <= 26; i++)
	{
		for(j = 0; j <= len; j++)  // dp[i][j]
		{
			for(k = 0; k <= cnt[i]; k++)  // i種類目の文字をk文字使う
			{
				if(j-k < 0) continue;
				dp[i][j] += dp[i-1][j-k] * nCk(j, k);
				dp[i][j] %= MOD;
			}
		}
	}
	
	ll ans = 0;
	for(j = 1; j <= len; j++)
	{
		ans += dp[26][j];
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
