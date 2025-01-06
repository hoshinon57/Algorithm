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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// https://atcoder.jp/contests/tdpc/tasks/tdpc_number

/*
 * 桁DPで解く。
 * EDPC-Sと全く同じ問題だったりする？
 * 
 * dp[i][sm][j]を
 *   i桁目まで見て、 (i=1～L)
 *   sm:未満フラグ (0:同じ, 1:未満)
 *   各桁の数値の総和をDで割った余りがjのときの、 (j=0～D-1)
 *   整数の種類数
 * として定義する。
 * ただし桁は直近2つだけあれば十分なことから、dp[sm][j]として定義して実装した。
 * DP遷移は実装、およびdp-digitDP.cppを参照。
 * 
 * 答は dp[0][0]+dp[1][0]-1. 
 * 値0は答に含まれないため-1している。
 */

int main(void)
{
	int D; cin >> D;
	string s; cin >> s;
	int L = (int)s.size();
	vector<vector<mint>> dp(2, vector<mint>(D, 0));
	dp[0][0] = 1;

	// 配るDP
	int i, sm, j;
	for(i = 0; i < L; i++)
	{
		vector<vector<mint>> ndp(2, vector<mint>(D, 0));
		for(sm = 0; sm < 2; sm++)
		{
			for(j = 0; j < D; j++)
			{
				const int v = s[i] - '0';  // 配る先の値
				int ed = (sm ? 9 : v);
				for(int d = 0; d <= ed; d++)  // 次に使う値
				{
					int nsm = (sm || d < v);
					int nj = (j+d) % D;
					ndp[nsm][nj] += dp[sm][j];					
				}
			}
		}
		swap(dp, ndp);
	}

	cout << (dp[0][0]+dp[1][0]-1).val() << endl;

	return 0;
}
