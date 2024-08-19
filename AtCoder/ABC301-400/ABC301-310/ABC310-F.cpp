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

// ABC310 https://atcoder.jp/contests/abc310

/*
 * 自力で解けず、解説を見た。
 * 確率DP&bitDP的な内容だった。
 * 
 * 基本は公式解説の通り。
 * dp[i][S]を以下のように定義する。
 *   Aのi個目まで見て、  (i=1～N)
 *   出目の和で作れる値、の集合がSになるような確率。
 *   S:値xを作れるとき、xbit目を1にする。x=0～10の範囲なので、全11bit.
 * dp[N][S]にて、Sの10bit目が立っているものの総和が答。
 * 
 * dp[i][j]からの遷移は、配るDPで以下となる。
 * (1)出目kが10以下のとき
 *    jをkbit左シフトしたものとjのORをnjとして、
 *    dp[i][j]/A[i+1] をdp[i+1][nj]へ加算
 * (2)出目kが10より大きいとき
 *    和は10までを考えればよいので、この出目は答に寄与しない。
 *    よって10より大きい出目を1つにまとめて、
 *    dp[i][j] * (a[i+1]-10)/a[i+1] をdp[i+1][j]へ加算  (njではなくjである点に注意)
 *         
 * [どう思考すれば解法にたどり着けるか]
 * ・最初、総和をDPのキーに持たせたものを考えたが、うまくいかなかった。
 * ・各要素を使う/使わないを選べるとき、作れる値(総和)の集合で考えてみる。
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

int main(void)
{
	// 1-indexed
	const ll B = 1<<11;  // 11:総和が0～10の11通り
	int i, j, k;
	ll N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<vector<ll>> dp(N+1, vector<ll>(B, 0));  // dp[N+1][1<<11]
	dp[0][1] = 1;  // bit0,つまり和0

	// 配るDP
	for(i = 0; i < N; i++)  // i->i+1への遷移 a[i+1]を使う
	{
		for(j = 1; j < B; j++)  // dp[i][j] -> dp[i+1][nj]
		{
			if(a[i+1] > 10)
			{
				dp[i+1][j] += dp[i][j] * (a[i+1]-10) % MOD * modinv(a[i+1], MOD) % MOD;
				dp[i+1][j] %= MOD;
			}
			for(k = 1; k <= min(10LL, a[i+1]); k++)  // 出目k
			{
				int nj = (j<<k)&(B-1);  // kビットシフトし、下位11bitのみ取得
				nj |= j;
				dp[i+1][nj] += dp[i][j] * modinv(a[i+1], MOD);
				dp[i+1][nj] %= MOD;
			}
		}
	}

	ll ans = 0;
	for(j = 0; j < B; j++)
	{
		if((j>>10)&1) ans = (ans + dp[N][j]) % MOD;
	}
	cout << ans << endl;

	return 0;
}
