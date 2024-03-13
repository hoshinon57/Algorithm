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

// 競プロ典型90問:11 https://atcoder.jp/contests/typical90/tasks/typical90_k

/*
 * 自力で解けず、解説を見た。
 * 区間スケジューリング問題として解く。
 * 
 * 基本は公式解説の通り。
 *   https://twitter.com/e869120/status/1381064464049401856
 * 締切の早いものから順に考えていくのが良い。
 * 仕事を締切順にソートして処理していく。
 * 
 * 報酬がどれも同一なら「締切の早い順に仕事をしていく」で良い。
 * 今回は報酬に差があるため、以下のDPを考える。
 * dp[i][j]
 *   i件目まで見て、 (i=1～N)
 *   j日目の終わりにおける、 (j=0～5000)
 *   報酬の最大
 * 
 * もらうDPで、遷移は
 *   dp[i][j] <- dp[i-1][j-C] + S   ただし j<=D
 * と書ける。
 * 
 * dp[N][*]の最大値が答。
 */

struct dt
{
	ll d, c, s;
};

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<dt> d(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> d[i].d >> d[i].c >> d[i].s;
	}
	sort(d.begin(), d.end(), [&](dt x, dt y){ return x.d < y.d; });

	vector<vector<ll>> dp(N+1, vector<ll>(5000+1, 0));  // dp[N+1][5000+1]
	// もらうDP
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= 5000; j++)
		{
			dp[i][j] = dp[i-1][j];  // 仕事iを選ばない場合

			// dp[i][j] <- dp[i-1][j-dt.c] + dt.s  ただしj<=dt.d
			if(j > d[i].d) continue;  // 締切過ぎた
			if(j-d[i].c >= 0)
			{
				chmax(dp[i][j], dp[i-1][j-d[i].c]+d[i].s);
			}
		}
	}

	ll ans = 0;
	for(j = 0; j <= 5000; j++)
	{
		chmax(ans, dp[N][j]);
	}
	cout << ans << endl;

	return 0;
}
