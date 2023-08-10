#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC135 https://atcoder.jp/contests/abc135

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   Sのi文字目まで見て、  (i:0～N)
 *   数値を13で割ったときの余りがjであるような  (j:0～12)
 *   組み合わせの数
 * 
 * 配るDPにてdp[i][j]からdp[i+1][*]への遷移を考えると、
 * (1)i+1文字目が数値の場合：
 *   i文字目までの余りを10倍し、(i+1)文字目を加算した要素が遷移先となる。
 *   つまりj*10+S[i+1].
 * (2)i+1文字目が'?'の場合：
 *   i文字目までの余りを10倍し、(i+1)文字目として0～9それぞれを考えた場合となる。
 * 
 * 答はdp[N][5].
 */

int main(void)
{
	// 1-indexed
	int i, j, k;
	string S; cin >> S;
	int N = (int)S.size();
	S = '_' + S;  // 1-indexedにしたい
	vector<vector<ll>> dp(N+1, vector<ll>(13, 0));  // dp[N+1][13]
	dp[0][0] = 1;
	
	// 配るDP
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < 13; j++)  // dp[i][j]からdp[i+1][*]へ遷移
		{
			if(S[i+1] != '?')
			{
				// dp[i][j]からの遷移先は、i文字目までの余りを10倍し、(i+1)文字目を加算した要素
				int n = (int)(S[i+1]-'0');
				int x = (j * 10 + n) % 13;
				dp[i+1][x] += dp[i][j];
				dp[i+1][x] %= MOD;
			}
			else
			{
				for(k = 0; k < 10; k++)  // ?が0～9それぞれの場合について計算
				{
					int x = (j * 10 + k) % 13;
					dp[i+1][x] += dp[i][j];
					dp[i+1][x] %= MOD;
				}
			}
		}
	}
	cout << dp[N][5] << endl;

	return 0;
}
