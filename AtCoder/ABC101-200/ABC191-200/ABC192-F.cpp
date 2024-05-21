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

// ABC192 https://atcoder.jp/contests/abc192

/*
 * 自力で解けず、解説を見た。
 * 動的計画法で解く。
 * 
 * 使う種類Kを固定して考えると、
 * K種類の素材を合成した総和をsumとしたときに、 sum+K*x=X が成り立つ必要がある。
 * つまりsumとXはKで割った余りが等しい。
 * ※制約より、素材を合成した時点でXを超えることは無い。
 * 
 * 余りに着目するケースでは、DPの添え字に余りを使うのが典型。
 * dp[i][j][l]を以下のように定義する。
 *   i個目まで見て、 (i=1～N)
 *   j個使っていて、 (j=0～K)
 *   それまで使った素材の総和を、Kで割った余りがl  (l=0～K-1)
 *   のときの総和の最大値。
 * 
 * (X-dp[N][K][X%K])/K が、このときの求める時間となる。
 * Kについて1～Nまでそれぞれ求めて、その最小値が答。
 * 
 * 計算量はO(N^4).
 * N=100のため10^8とかになるが、DP遷移の処理は軽いため十分に間に合う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・kずつ増えていく仕組みより、選んだ素材の総和とXは、kで割った余りが等しい。
 *   余りに着目するケースでは、DPの添え字に余りを使うのが典型。
 * ・kについて全探索すれば良さそう。
 */

int main(void)
{
	ll i, j, l;
	ll N, X; cin >> N >> X;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}

	// K個選ぶとき、ちょうどXのポーションを作れる組み合わせにて、その総和の最大値を返す
	auto func = [&](ll K) -> ll
	{
		vector dp(N+1, vector(K+1, vector<ll>(K, -INF64)));
		dp[0][0][0] = 0;
		for(i = 1; i <= N; i++)  // a[i]
		{
			for(j = 0; j <= K; j++)
			{
				for(l = 0; l < K; l++)
				{
					dp[i][j][l] = dp[i-1][j][l];  // i個目を使わないケース
					// i個目を使うケース
					if(j-1 >= 0)
					{
						int prev = (l-(a[i]%K)+K)%K;
						chmax(dp[i][j][l], dp[i-1][j-1][prev]+a[i]);
					}
				}
			}
		}
		ll t = dp[N][K][X%K];
		if(t < 0) return INF64;  // ちょうどXになる組み合わせが無い
		return (X-t) / K;
	};

	ll ans = INF64;
	for(ll k = 1; k <= N; k++)
	{
		ll tmp = func(k);
		chmin(ans, tmp);
	}
	cout << ans << endl;

	return 0;
}
