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

// ABC346 https://atcoder.jp/contests/abc346

/*
 * 動的計画法で解く。
 * 
 * dp[i][j][k]を以下のように定義する。
 *   i文字目まで見て、 (i=1～N)
 *   i文字目をjにしたい場合に、 (j=0,1)
 *   前後の文字が k=0:まだ一致していない k=1:既に一致したことがある
 *   ときの、コスト最小値。
 * min(dp[N][0][1], dp[N][1][1])が答。
 * 
 * k=0のとき、dp[i-1][1-j][0]から遷移する。
 * dp[*][*][k=1]からは遷移できない。
 * 
 * k=1のとき、dp[*][*][k=0 or k=1]のそれぞれから遷移する。
 * 前者はjが同じ値から、後者はjが反対側の値から遷移する。
 * 
 * k=1の遷移について、1文字目の時点では「前後の文字が一致した」はありえない点に注意。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	string s; cin >> s;
	vector<ll> c(N+1); for(i = 1; i <= N; i++) {cin >> c[i];}  // 1-indexed
	vector dp(N+1, vector(2, vector<ll>(2, INF64)));
	dp[0][0][0] = dp[0][1][0] = 0;
	
	// もらうDP
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j < 2; j++)  // dp[i][j][*]
		{
			ll cost = 0;
			if((s[i-1]-'0') != j) cost = c[i];

			// k=0
			chmin(dp[i][j][0], dp[i-1][1-j][0]+cost);
			
			// k=1
			if(i==1) {;}  // 1文字目の時点では「前後の文字が一致した」はありえない
			else
			{
				chmin(dp[i][j][1], dp[i-1][j][0]+cost);
				chmin(dp[i][j][1], dp[i-1][1-j][1]+cost);
			}
		}
	}
	ll ans = min(dp[N][0][1], dp[N][1][1]);
	cout << ans << endl;

	return 0;
}
