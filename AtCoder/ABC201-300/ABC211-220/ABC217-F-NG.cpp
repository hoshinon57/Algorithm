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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC217 https://atcoder.jp/contests/abc217

/*
 * 後に考えたがダブリがありNGの方針。供養/備忘録として残しておく。
 * 
 * 典型的（？）な区間DPとして考えて、
 * dp[l][r]を区間[l,r)における操作方法の場合の数とする。
 * 
 * ・両端を取るケース
 *   人l,rが仲良しなら、dp[l+1][r-1]
 * ・左からi個、以降(size-i)個取るケース
 *   dp[l][l+i] * dp[l+1][r] * C((r-l)/2, i/2)
 * を加算していく。
 * …という方針を考えたがWA.
 * 
 * よくよく考えると、6人で 1-2,3-4,5-6 が仲良しのとき、
 *   1-2 / 3-4,5-6 で分割ケース
 *   1-2,3-4 / 5-6 で分割ケース
 * とで、同じ操作方法がダブってしまう。
 */

const ll MOD = 998244353;
const ll NCK_SIZE = 5000;  // N<=2*10^5 など、問題文の制約に合わせる
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

vector<vector<bool>> ok;  // ok[a][b]:人a,bが仲良しならtrue
vector<vector<mint>> dp;  // dp[l][r]: [l,r)での操作数
vector<vector<bool>> chk;

// dp[l][r]を返す
mint rec(int l, int r)
{
	auto &d = dp[l][r];
	if(chk[l][r]) return d;  // メモ済
	d = 0;

	// ベースケース
	if(l == r) { return (d=0); }  // 念のため
	if(r-l == 2)
	{
		if(ok[l][l+1]) d = 1;
		return d;
	}

	// 両端を取るケース (両端は最後に取る)
	if(ok[l][r-1])
	{
		d += rec(l+1, r-1);
	}

	// 左からi個, 以降(size-i)個取るケース
	for(int i = 2; l+i < r; i+=2)
	{
		mint tmp1 = rec(l, l+i);
		mint tmp2 = rec(l+i, r);
		mint tmp3 = nCk((r-l)/2, i/2);
		d += tmp1 * tmp2 * tmp3;
	}

	chk[l][r] = true;
	return d;
}

int main(void)
{
	nCk_init();

	int i;
	int N, M; cin >> N >> M;
	N *= 2;
	ok.resize(N, vector<bool>(N, false));
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		ok[a][b] = ok[b][a] = true;
	}
	dp.resize(N+1, vector<mint>(N+1, 0));
	chk.resize(N+1, vector<bool>(N+1, false));
	cout << rec(0, N).val() << endl;
	return 0;
}
