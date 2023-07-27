#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;
const ll MOD = 998244353;

// ABC263 https://atcoder.jp/contests/abc263

/*
 * コンテスト中に解けず、解説を見て実装した。
 * 期待値DPにて解く。
 * 参考：
 *   https://kyopro-friends.hatenablog.com/entry/2019/01/12/231000
 *   https://blog.hamayanhamayan.com/entry/2019/01/09/001607
 *   https://twitter.com/kyopro_friends/status/1555914095907713024
 * 
 * 後ろのマスから前のマスへと計算していく。
 * 例として、A(2)=1のときのマス2を考える(サイコロの目は0 or 1).
 * dp[i]:マスiからマスNへ行くまでにサイコロを振る回数の期待値 とすると、
 *   dp[2] = ((dp[3]+1)* 1/2) + ((dp[2]+1)* 1/2)
 * となる。+1はマス2にてサイコロを1回振ることを表す。
 * 右辺にもdp[2]があるので、これを整理して消すと
 *   dp[2] = dp[3] + 2
 * となる。
 * 
 * これを一般化すると
 *   dp[i] = (Σ{j=0 to Ai}(dp[i+j]+1)* 1/(Ai+1))
 * となる。同じく整理すると
 *   dp[i] = (Σ{j=1 to Ai}(dp[i+j]))* 1/Ai + (Ai+1)/Ai
 * となる。
 * Σの部分は、dp[i]～dp[N]までの累積和をdp_sum[]として計算しておく。
 * あとは上記をi=N-1から1まで、順に計算していけばよい。
 * 
 * 問題文の"注記"の意味が分からなかったが、これは単純に
 *   期待値が 5/2 であれば、5*(2の逆元) の計算をする
 * という意味で受け取って良さそう。
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
	ll i;
	ll N;
	cin >> N;
	vector<ll> a(N+1);
	for(i = 1; i <= N-1; i++)
	{
		cin >> a[i];
	}

	// dp[i]:マスiからマスNへ行くまでにサイコロを振る回数の期待値
	// i:1～N
	vector<ll> dp(N+1);
	dp[N] = 0;  // ゴールのマスは0回
	// dpsum[i]:dp[i]～dp[N]までの累積和
	// i:1～N  かつ楽に計算するため[N+1]も使いたい
	vector<ll> dpsum(N+2);  // 楽に計算するためdpsum[N+1]を使いたい そのため(N+2)にする
	dpsum[N] = dp[N];

	// マスN-1から手前方向に進めていく
	for(i = N-1; i >= 1; i--)
	{
		ll dpsum_tmp;
		ll ai_inv = modinv(a[i], MOD);
		dpsum_tmp = (dpsum[i+1] + MOD - dpsum[i+a[i]+1])%MOD;  // i=N-1のとき[i+a[i]+1]はN+1になる dpsum[N+1]は範囲内で、かつ0
		dp[i] = (dpsum_tmp * ai_inv)%MOD + ((a[i]+1)*ai_inv)%MOD;
		dp[i] %= MOD;
		dpsum[i] = (dpsum[i+1] + dp[i])%MOD;
	}

	cout << dp[1] << endl;

	return 0;
}
