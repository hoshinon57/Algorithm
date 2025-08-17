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

// ABC419 https://atcoder.jp/contests/abc419

/*
 * DPで解く。
 *   https://x.com/kyopro_friends/status/1956713441303146924
 * 
 * 条件(1)
 * Si=a[i]+a[i+1]+...+a[i+L-1] と、a[i]からL個の和とすると、
 * Si ≡ S(i-1) mod M である必要がある。式変形すると
 * a[i+L-1] ≡ a[i-1]
 * よって、全てのiで a[i+L] ≡ a[i] (mod M)となる必要がある。
 * つまり、L個ごとにグループ分けしたときに、グループ内のa[i]はMで割ったあまりが等しくなる必要がある。
 * 
 * 条件(2)
 * a[0]+a[1]+...+a[L-1] ≡ 0 (mod M)
 * 
 * 以上より、まずはcost[i][j]を以下のように定義する。
 *   L個ごとにグループ分けしたときの、i番目のグループにて、 (i=0～L-1)
 *   グループ内の総和をj(mod M)にするときの、 (j=0～M-1)
 *   操作コスト最小値
 * これは事前計算しておく。
 * 
 * 次にdp[j]を以下のように定義する。
 *   a[i]まで見て、 (i=0～L-1)
 *   (a[0]+a[1]+...+a[j])%M = j のときの、(j=0～M-1)
 *   操作コスト最小値
 * 条件(2)より、dp[0]が答。
 * 
 * DP遷移は、各dp[j]に対して
 *   a[i]のグループをk(mod M)にそろえる
 * としてkについて全探索が必要なので、O(L*M^2)となる。詳細は実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・連続するL個について考えるとき、a[i],a[i+L],a[i+2L],... のようにL個ごとに着目してみる。
 */

int main(void)
{
	ll i, j, k;
	ll N, M, L; cin >> N >> M >> L;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<vector<ll>> cost(L, vector<ll>(M, 0));  // cost[i][j] i番目のグループを、その総和をj(mod M)に揃えるコスト
	for(i = 0; i < N; i++)  // i%L
	{
		for(j = 0; j < M; j++)
		{
			ll tmp = (j-a[i]+M)%M;
			cost[i%L][j] += tmp;
		}
	}

	vector<ll> dp(M, INF64);
	dp[0] = 0;
	for(i = 0; i < L; i++)  // a[i]
	{
		vector<ll> ndp(M, INF64);
		for(j = 0; j < M; j++)  // そこまでのa[]の和 mod M
		{
			for(k = 0; k < M; k++)  // a[i]のグループを、k(mod M)に揃える
			{
				ll pj = (j-k+M)%M;
				chmin(ndp[j], dp[pj]+cost[i][k]);
			}
		}
		swap(dp, ndp);
	}
	cout << dp[0] << endl;

	return 0;
}
