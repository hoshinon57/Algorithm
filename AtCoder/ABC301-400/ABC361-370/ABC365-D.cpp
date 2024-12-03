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

// ABC365 https://atcoder.jp/contests/abc365

/*
 * dp[i][j]を
 *   i手目に手jを出したときの、勝ち数最大
 *   i=1～N
 *   j=0:R, 1:P, 2:S
 * として定義してDPすればよい。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	string s; cin >> s;

	vector<vector<int>> dp(N+1, vector<int>(3, 0));  // dp[N+1][3]
	for(i = 1; i <= N; i++)
	{
		int ak;  // Aokiの手
		if(s[i-1] == 'R') ak = 0;
		else if(s[i-1] == 'P') ak = 1;
		else ak = 2;

		for(j = 0; j < 3; j++)  // dp[i][j]を求める
		{
			int res = (j-ak+3)%3;  // 勝敗
			if(res == 0)  // 引き分け
			{
				dp[i][j] = max(dp[i-1][(j+1)%3], dp[i-1][(j+2)%3]);  // 同じ手は2連続で出せないので、[j]とは異なる添字から選ぶ
			}
			else if(res == 1)  // 勝ち
			{
				dp[i][j] = max(dp[i-1][(j+1)%3], dp[i-1][(j+2)%3]) + 1;
			}
			else  // 負け
			{
				dp[i][j] = -INF32;
			}
		}
	}
	cout << *max_element(dp[N].begin(), dp[N].end()) << endl;

	return 0;
}
