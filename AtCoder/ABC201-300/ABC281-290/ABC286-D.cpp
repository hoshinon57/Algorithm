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

// ABC286 https://atcoder.jp/contests/abc286

/*
 * 動的計画法で解く。O(NX)解法。
 * 
 * dp[i][j]を以下のように定義する。
 *   i種類目の硬貨まで見て、 (i=1～N)
 *   ちょうどj円を支払えるときの、 (j=0～X)
 *   i種類目に使う硬貨の枚数
 *   ちょうどj円支払えないとき、dp=INF. i種類目の硬貨を使わなくともj円を払えるケースもあり、そのときはdp=0.
 * dp[N][X]がINFか否かでYes/Noが決まる。
 * 
 * もらうDPにて、DP遷移は以下のようになる。
 * (1)Ai種類目の硬貨を使わないケース
 *   手前の種類の硬貨まででj円を支払える場合、dp[i][j]=0.
 *   そうでなければ、dp[i][j]=INF. となる。
 * (2)Ai種類目の硬貨を使うケース
 *   いつもはdp[i-1][*]から遷移するが、今回はdp[i][*]から遷移する。
 *   dp[i][j-a[i]] < b[i] であれば、そこからAi種類目の硬貨を1枚追加できる。
 *   この場合、chmin(dp[i][j], dp[i][j-a[i]]+1) となる。
 */

bool solve(void)
{
	int i, j;
	int N, X; cin >> N >> X;
	vector<int> a(N+1), b(N+1); for(i = 1; i < N+1; i++) {cin >> a[i] >> b[i];}  // 1-indexed
	vector<vector<int>> dp(N+1, vector<int>(X+1, INF32));  // 初期値INF
	dp[0][0] = 0;

	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= X; j++)  // dp[i][j]
		{
			if(dp[i-1][j] != INF32) dp[i][j] = 0;  // 1つ前の種類の硬貨まででj円を払えるので、DP=0.

			if(j-a[i] >= 0 && dp[i][j-a[i]] < b[i])  // まだB枚まで使ってない
			{
				chmin(dp[i][j], dp[i][j-a[i]]+1);  // dp[i-1]ではなく、dp[i][*]からの遷移
			}
		}
	}

	return (dp[N][X] != INF32);
}

int main(void)
{
	YesNo(solve());
	return 0;
}
