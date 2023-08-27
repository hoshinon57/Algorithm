#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC312 https://atcoder.jp/contests/abc312

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i文字目まで見て、  (i=1~|S|)
 *   '('の数 - ')'の数がjであるときの、  (j=0～|S|)
 *   文字列の種類数
 * 答は dp[|S|][0].
 * j<0になった時点で条件を満たすことは無くなるため、j>=0のみ計算すればよい。
 * 
 * もらうDPにて、DPの遷移は以下。
 * ・i文字目が'('または')'のとき：
 *   それぞれ、dp[i-1][j-1], dp[i-1][j+1]の値を加算する。
 * ・i文字目が'?'のとき：
 *   '(',')'どちらにも使えるので、dp[i-1][j-1]+dp[i-1][j+1]を加算する。
 */

ll dp[3005][1505];

int main(void)
{
	int i, j;
	string S; cin >> S;
	int L = (int)S.size();

	dp[0][0] = 1;  // DP初期値
	for(i = 1; i <= L; i++)
	{
		char c = S[i-1];
		for(j = 0; j <= 1500; j++)  // |S|=3000の最大ケースにて、jが1500を超えた時点で最終的に条件を満たすことはないため、計算不要となる
		{
			switch(c)
			{
			case '(':
				if(j-1 >= 0) dp[i][j] += dp[i-1][j-1];
				break;
			case ')':
				dp[i][j] += dp[i-1][j+1];
				break;
			case '?':
				if(j-1 >= 0) dp[i][j] += dp[i-1][j-1] + dp[i-1][j+1];
				else dp[i][j] += dp[i-1][j+1];
				break;
			}
			dp[i][j] %= MOD;
		}
	}
	cout << dp[L][0] << endl;

	return 0;
}
