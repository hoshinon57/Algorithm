#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:08 https://atcoder.jp/contests/typical90/tasks/typical90_h

/*
 * 解説と同じ考え方。
 *   https://twitter.com/e869120/status/1379927227739987972
 *   https://drken1215.hatenablog.com/entry/2022/04/01/121700
 * 
 * dp[i][j]を以下のように定義して解く。
 *   Sのi文字目まで見て、  (i=1～N)
 *   ここまでで"atcoder"のj文字目までが作れるような、  (j=1～7)
 *   取り出し方
 * 答はdp[N][7].
 * 
 * [ACまでの思考の流れ]
 * ・例えばSの5文字目まで見終わったときに、
 *   以降の文字を見るにあたって必要な情報は「これまでに"atcoder"の何文字目まで作れる？」であり、
 *   "at"までが{1,4}番目と{1,5}番目のような取り出し方の詳細情報は「別にどうでもいい」となる。
 *   であれば、「これまでに作れる文字数」でDPができる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	string s; cin >> s;
	string atc = "atcoder";
	vector<vector<ll>> dp(N+1, vector<ll>(7+1, 0));  // dp[N+1][7+1]
	dp[0][0] = 1;
	// もらうDP
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= 7; j++)
		{
			dp[i][j] = dp[i-1][j];
			if(j-1 >= 0 && s[i-1] == atc[j-1])  // Sのi文字目とatcのj文字目が等しい (indexedの違いにより、-1している)
			{
				dp[i][j] += dp[i-1][j-1];
				dp[i][j] %= MOD;
			}
		}
	}
	cout << dp[N][7] << endl;

	return 0;
}
