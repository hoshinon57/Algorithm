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

// ABC325 https://atcoder.jp/contests/abc325

/*
 * 自力で解けず、解説を見た。動的計画法で解く。
 *   https://atcoder.jp/contests/abc325/editorial/7449
 *   https://www.youtube.com/watch?v=KiujjWaQJpk
 * 
 * 公式解説の通りで、単純なDPを考えると
 *   「i個目の区間まで見て」「センサー1をj個使って」「センサー2をk個使った」ときに
 *   i個目の区間までをカバーできているか？
 * となる。
 * ただしこれだと状態数だけで10^8であり、遷移数も考えると到底間に合わない。
 * 
 * よく考えると「センサー2をy個使った」という情報は不要で、
 *   「i個目の区間まで見て」「センサー1をj個使って」
 *   i個目の区間までをカバーするようにしたときの、センサー2が必要な最小値
 * とすれば、状態数が10^5, 遷移が10^3で全体で10^8で間に合う。
 * ※10^8はギリギリっぽいが、ループ内処理が単純のため十分に間に合う。
 * 
 * dp[N][*]<=k2である要素のうち、コスト最小値が答となる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・DPの状態数（次元数）を減らせないか？と考えてみる。
 *   単純に考えたときに、DP値が「できる or できない」のbool値だった場合、
 *   状態数を1つ減らし、DP値をその減らした状態数に使えないかを考えてみる。
 */

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

int main(void)
{
	ll i, j, m;
	ll N; cin >> N;
	vector<ll> d(N+1); for(i = 1; i <= N; i++) {cin >> d[i];}
	ll l1, c1, k1; cin >> l1 >> c1 >> k1;
	ll l2, c2, k2; cin >> l2 >> c2 >> k2;

	// dp[i][j]
	//   i個目の区間まで見て、  (i=1～N)
	//   センサー1をj個使ったときに、  (j=0～K1)
	//   それまでの区間をカバーするようにしたときの、センサー2の必要数の最小値
	vector<vector<ll>> dp(N+1, vector<ll>(k1+1, INF64));  // dp[N+1][k1+1]
	dp[0][0] = 0;
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= k1; j++)  // dp[i][j]を求める
		{
			for(m = 0; m <= k1; m++)  // センサー1をm個使うケース
			{
				if(j-m < 0) continue;
				ll x = max(d[i]-l1*m, (ll)0);
				x = ceil_div(x, l2);  // センサー2が必要な個数
				chmin(dp[i][j], dp[i-1][j-m]+x);
			}
		}
	}

	ll ans = INF64;
	for(j = 0; j <= k1; j++)  // センサー1をj個, センサー2をdp[N][j]個使う
	{
		if(dp[N][j] > k2) continue;  // センサー2はK2個まで
		chmin(ans, j*c1 + dp[N][j]*c2);
	}
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
