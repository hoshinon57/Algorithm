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

// ABC189 https://atcoder.jp/contests/abc189

/*
 * DPで解いた。
 * 以下の解説と同様の実装。
 *   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2021/0123_abc189
 * 
 * dp[i][j]を
 *   xiまで見たときに、  (i:0～N)
 *   yi(それまでの論理演算の結果)が  j=0:false, j=1:true
 *   になるような場合の数
 * 直近で使うSが、
 *   AND:y(i-1),xiがともにtrueのときのみyi=true, 他はfalse
 *   OR:y(i-1),xiがともにfalseのときのみyi=false, 他はtrue
 * となるので、それをDP遷移に用いればよい。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<string> s(N); for(i = 0; i < N; i++) {cin >> s[i];}
	vector<vector<ll>> dp(N+1, vector<ll>(2, 0));  // dp[N+1][2]
	dp[0][0] = dp[0][1] = 1;
	for(i = 1; i <= N; i++)
	{
		if(s[i-1] == "AND")
		{
			dp[i][0] = dp[i-1][0]*2 + dp[i-1][1];
			dp[i][1] = dp[i-1][1];  // y(i-1)がtrue, xiもtrueのイメージ
		}
		else  // "OR"
		{
			dp[i][0] = dp[i-1][0];  // y(i-1)がfalse, xiもfalseのイメージ
			dp[i][1] = dp[i-1][0] + dp[i-1][1]*2;
		}
	}
	cout << dp[N][1] << endl;

	return 0;
}
