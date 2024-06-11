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

// ABC333 https://atcoder.jp/contests/abc333

/*
 * 自力で解けず、解説を見た。いやーこれ難しい…
 * DPで解く。
 * 
 * dp[i][j]を以下のように定義する。1-indexed.
 *   i人いて、(i=1～N)
 *   先頭からj人目が残るときの確率 (j=1～i)
 * iが小さい方から求めていき、dp[N][*]が答。
 * DP初期値はdp[1][1]=1.
 * 
 * 遷移を考えると、1/2でずっと列の末尾に動くケースにて循環する。まずはこの処理を考える。
 * 例えばi=3のときを考えてみると、
 *   dp[3][1] = (1+1/8+1/64+...) * (       + 1/4*dp[2][2] + 1/8*dp[2][1])
 * となる。右辺をX*Yと置く。
 * Xは等比数列の和から整理すると、 X=2^3/(2^3-1) となる。
 * Yをdp[3][2], dp[3][3]についても考えてみると、
 *   (1/2*dp[2][1] +              + 1/8*dp[2][2])
 *   (1/2*dp[2][2] + 1/4*dp[2][1] +             )
 * となる。
 * これを愚直に計算すると全体でO(N^3)となり間に合わないが、
 * dp[3][*]の差分は小さいことから、
 *   dp[3][1]のみ愚直に計算し、
 *   dp[3][2]以降は差分に着目してO(1)で計算する[典型]
 * により、O(N^2)に抑えられる。
 * 
 * 具体的には、dp[3][1]のときのYから、
 *   1/8の項を引いて消す
 *   Y全体を1/2倍する
 *   先ほど引いた部分を、1/2*(***) として加算する
 * とすれば、dp[3][2]のYが求まる。
 * 
 * 上記の空白部分は、dp[2][3] として「ありえない」値を指定すると計算しやすい。
 * (dp[2][3]=0 になるので上手くいく)
 * 
 * ※文章だけだと説明しづらいので、紙に色々書くと整理しやすかった。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・単純なDPを考えるとO(N^3)でNG.
 *   差分が小さいことに着目すると[典型], O(N^2)に抑えられる。
 * ・見た目でDPっぽい。確率DPとか期待値DPとか。
 *   制約的にO(N^2)とかになりそう。
 *   ここからメタ読みしてdp[i][j]の定義を見つけられるか。
 */

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

// p^0～p^(n-1)を事前計算して返す
// mが0以外の場合、mで割った余りを設定する
template<typename T>
vector<T> cal_pow_inadv(T p, int n, T m = 0)
{
	vector<T> ret(n);
	ret[0] = (T)1;
	for(int i = 1; i < n; i++) {
		ret[i] = ret[i-1] * p;
		if(m != 0) ret[i] %= m;
	}
	return ret;
}

int main(void)
{
	ll i, j;
	ll N; cin >> N;
	vector<ll> p2(N+1);  // p2[i]:2^i
	p2 = cal_pow_inadv<ll>(2, N+1, MOD);
	vector<ll> p2inv(N+1);  // p2inv[i]:1/(2^i)
	for(i = 0; i <= N; i++) p2inv[i] = modinv(p2[i], MOD);

	vector<vector<ll>> dp(N+1, vector<ll>(N+1, 0));  // dp[N+1][N+1]
	dp[1][1] = 1;
	for(i = 2; i <= N; i++)  // dp[i][1]～dp[i][i]を求める
	{
		ll x, y;
		x = p2[i] * modinv((p2[i]-1+MOD)%MOD, MOD);  // 2^i / (2^i)-1
		x %= MOD;
		for(j = 1; j <= i; j++)
		{
			if(j == 1)  // 初回のみ愚直にYを計算
			{
				y = 0;
				for(ll b = i; b > 0; b--)
				{
					y += p2inv[i-b+1] * dp[i-1][b];
					y %= MOD;
				}
			}
			dp[i][j] = x*y%MOD;

			// 次のyを計算 詳細はファイル先頭のコメントを参照
			y = y - (p2inv[i] * dp[i-1][j] % MOD) + MOD;
			y %= MOD;
			y *= p2inv[1];
			y %= MOD;
			y += p2inv[1] * dp[i-1][j] % MOD;
			y %= MOD;
		}
	}

	for(j = 1; j <= N; j++)
	{
		cout << dp[N][j];
		if(j != N) cout << " ";
	}
	cout << endl;

	return 0;
}
