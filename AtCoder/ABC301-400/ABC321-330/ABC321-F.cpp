#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC321 https://atcoder.jp/contests/abc321

/*
 * コンテスト中に解けず、解説を見た。
 * 動的計画法で解く。(戻すDP)
 * 
 * dp[i][j]を以下のように定義する。
 *   i番目のクエリまで見て、  (i:1～Q)
 *   いくつかを選んだ和がjになるような  (j:0～K)
 *   組み合わせの総数
 * 
 * クエリが"+x"だけならば、普通の部分和問題で解ける。
 * "-x"をどうするか、が問題。
 * 制約より、全体でO(QK), クエリごとにO(K)に処理したい。
 * 
 * 色々と手で書いてみると、"-x"のクエリは
 *   dp[i][j] -= dp[i][j-x]
 * で作れることが分かる。
 * ただしjが小さい方から処理する。右辺は[i-1]ではなく[i]である点に注意。
 * 
 * なぜこれで良いんだろうか(特に右辺)…と色々悩んだが、たぶんこうなんだろう、という理解が限界だった。
 * またいずれ考えてみよう。
 * 参考になりそうな記事は以下。
 *   https://drken1215.hatenablog.com/entry/2023/09/25/070100
 */

int main(void)
{
	ll i, j;
	ll Q, K; cin >> Q >> K;

	vector<vector<ll>> dp(Q+1, vector<ll>(K+1, 0));  // dp[Q+1][K+1]
	dp[0][0] = 1;

	for(i = 1; i <= Q; i++)
	{
		string qe;
		ll x;
		cin >> qe >> x;
		if(qe == "+")
		{
			for(j = 0; j <= K; j++)  // 総和j
			{
				dp[i][j] = dp[i-1][j];
				if(j-x >= 0)  // xを使うケース
				{
					dp[i][j] += dp[i-1][j-x];
					dp[i][j] %= MOD;
				}
			}
		}
		else  // "-"
		{
			for(j = 0; j <= K; j++)
			{
				dp[i][j] = dp[i-1][j];  // xを使わないケース
				if(j-x >= 0)  // xを使うケース
				{
					dp[i][j] -= dp[i][j-x];
					dp[i][j] = (dp[i][j]+MOD)%MOD;
				}
			}
		}
		cout << dp[i][K] << endl;
	}

	return 0;
}
