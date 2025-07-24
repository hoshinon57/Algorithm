#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

// ABC336 https://atcoder.jp/contests/abc336

/*
 * コンテスト中に解けず、解説を見た。
 * 桁DPで解く。
 * 
 * 桁和sを固定して考える。
 * 制約より、最大のsは9*14=136であり、s=1～136それぞれについて計算する。
 * 
 * dp[i][j][k][smaller]を以下のように定義する。
 *   i桁目までを見たときに、  (i=1～N.size)
 *   それまでの桁和がjで、 (j=0～s)
 *   それまでの値を10進数で見たときに、sで割った余りがkで、 (k=0～s-1)
 *   それまでの値が、smaller=0:Nと等しい 1:Nより小さい
 *   ときの組み合わせの数
 * これを配るDPにて実装する。
 * 
 * dp初期値はdp[0][0][0][0]=1.
 * 答はdp[N.size][s][0][0]+dp[N.size][s][0][1].
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・桁に関する条件を数え上げる⇒桁DPを考えてみる
 * ・DPにおいて必要な情報として、
 *   ・どの桁まで見ているか
 *   ・そこまでの桁和
 *   ・桁和で割り切れるかを知りたい⇒そこまでの値を桁和で割った余り
 *   ・そこまでの値がNと等しいかNより小さいか(桁DP)
 *   を区別すれば状態は網羅できそう。これをdp[]にて表現する。
**/

// (2025/7/24)実装を更新。
// dp/ndp形式にし、dp[sm][i][j]を
// sm:smaller, i:そこまでの桁和, j:そこまでの値を(桁和)で割った余り
// とした。

int main(void)
{
	string s; cin >> s;
	int L = (int)s.size();
	const ll mxk = 9*14;  // 桁和の最大値

	// 桁和=kについて解く
	auto solve = [&](ll k) -> ll
	{
		vector dp(2, vector(k+1, vector<ll>(k, 0)));
		dp[0][0][0] = 1;
		ll sm, i, j, d;
		for(ll _ = 0; _ < L; _++)
		{
			vector ndp(2, vector(k+1, vector<ll>(k, 0)));
			for(sm = 0; sm < 2; sm++)
			{
				for(i = 0; i <= k; i++)
				{
					for(j = 0; j < k; j++)  // dp[sm][i][j]から配る
					{
						const ll D = s[_] - '0';
						ll ed = (sm ? 9 : D);
						for(d = 0; d <= ed; d++)
						{
							ll nsm = (sm || d < D);
							ll ni = i+d;
							ll nj = (j*10+d) % k;
							if(ni > k) continue;  
							ndp[nsm][ni][nj] += dp[sm][i][j];
						}
					}
				}
			}
			swap(dp, ndp);
		}
		return dp[0][k][0] + dp[1][k][0];
	};

	ll ans = 0;
	for(ll k = 1; k <= mxk; k++)  // 桁和kを固定
	{
		ans += solve(k);
	}
	cout << ans << endl;

	return 0;
}
