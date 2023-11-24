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

// ABC219 https://atcoder.jp/contests/abc219

/*
 * 動的計画法で解く。各種解説の通り。
 *   https://blog.hamayanhamayan.com/entry/2021/10/10/151003
 * 
 * dp[i][j][k]を以下のように定義する。
 *   i番目のお弁当まで見たときに、  (i:0～N)
 *   たこ焼きをj個、たい焼きをk個持っているときの、  (j:0～X, k:0～Y)
 *   購入するお弁当の数の最小値
 *   ★ただし、欲しい数より多いたこ焼きたい焼きは他の人にあげてしまうこととする。
 * 
 * DP遷移は以下の通り。
 * ・i番目のお弁当を買わない場合
 *   dp[i][j][k] = dp[i-1][j][k]
 * ・i番目のお弁当を買う場合
 *   dp[i][j][k]: dp[i-1][j-A][k-B]+1との最小値
 *   ただしj-A,k-Bは下限0でクリップする。
 * 
 * dp[N][X][Y]が答。
 * 計算量はO(NXY).
 */

int main(void)
{
	int i, j, k;
	int N, X, Y; cin >> N >> X >> Y;
	vector dp(N+1, vector(X+1, vector<ll>(Y+1, INF32)));
	dp[0][0][0] = 0;
	for(i = 1; i <= N; i++)  // i番目のお弁当
	{
		int a, b; cin >> a >> b;
		for(j = 0; j <= X; j++)
		{
			for(k = 0; k <= Y; k++)
			{
				dp[i][j][k] = dp[i-1][j][k];
				int na = max(j-a, 0);
				int nb = max(k-b, 0);
//				dp[i][j][k] = min(dp[i][j][k], dp[i-1][na][nb] + 1);
				chmin(dp[i][j][k], dp[i-1][na][nb]+1);
			}
		}
	}

	int ans = dp[N][X][Y];
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
