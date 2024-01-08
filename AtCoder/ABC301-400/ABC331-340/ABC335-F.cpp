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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC335 https://atcoder.jp/contests/abc335

/*
 * 自力で解けず、解説を見た。
 *   https://atcoder.jp/contests/abc335/editorial/9038
 * 
 * 公式解説に沿った実装。
 * dp[i]を「iマス目で終わるときの、黒マスの集合の組み合わせ数」と定義する。
 * これは結局、配るDPにて
 *   dp[i+a[i]*k] += dp[i]
 * を各i,kについて繰り返したものなる。
 * 
 * a[i]が大きい場合は上記で良い。
 * ただしa[i]が1のように小さい場合、計算量がO(N^2)とかになり間に合わない。
 * 
 * よってa[i]の大小によってやり方を変えてみる。
 * (1)a[i]<XX のとき
 *   マスiから +a[i], +a[i]*2, +a[i]*3, ... のように dp[]に加算していくことになる。
 *   ここでcnt[x][y]を「マスiについて、iをxで割った余りがyのときに、dpに加算する量」と定義する。
 *   すると各dp[i]について、1～XXまでcnt[][]からもらって加算すればdp[i]が求まる。
 *   (cntの更新も必要)
 * (2)a[i]>=XX のとき
 *   これは前述の配るDP.
 * 
 * XXをどう決めるかについては、計算量がdp[i]あたり O(XX+N/XX) になることから、
 * XX=√Nにするのが最適っぽい。
 * 実装では √maxN=500 とした。
 * 
 * Σdp[]が答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・最初、配るDPでの遷移だけを思いついたものの、計算量の関係で断念した。
 *   ここでもらうDPで解く場合も考えるべきだった。
 * ・もらうDPも思いついた前提だが、
 *   配るDP：a[i]が小さいときに不利
 *   もらうDP：a[i]が大きいときまでカバーしようとすると不利
 *   であることから、両者のミックスで解けないかを考える。
 */

int main(void)
{
	// 0-indexed
	ll i, j;
	ll N; cin >> N;
	const ll XX = 500;  // もらう/配るDPの境界となるa[i]の値
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> dp(N);
	dp[0] = 1;
	vector<vector<ll>> cnt(XX, vector<ll>(XX, 0));  // マスiをxで割った余りがy
	for(i = 0; i < N; i++)
	{
		// もらうDPにて自身に加算
		for(j = 1; j < XX; j++)
		{
			dp[i] += cnt[j][i%j];
			dp[i] %= MOD;
		}

		if(a[i] < XX)
		{
			// もらうDP用の更新
			cnt[a[i]][i%a[i]] += dp[i];
			cnt[a[i]][i%a[i]] %= MOD;
		}
		else
		{
			for(j = i+a[i]; j < N; j+=a[i])  // 配るDPにて相手を加算していく
			{
				dp[j] += dp[i];
				dp[j] %= MOD;
			}
		}
	}

	ll ans = 0;
	for(auto &e : dp)
	{
		ans += e;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
