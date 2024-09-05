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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC237 https://atcoder.jp/contests/abc237

/*
 * 自力で解けず、解説を見た。LISの気持ちになって解く。
 *   https://scrapbox.io/ganariya-competitive/AtCoderBeginnerContest237_F%E5%95%8F%E9%A1%8C500%E7%82%B9_%E3%80%8C%7CLIS%7C_=_3%E3%80%8D
 *   https://bondo.hateblo.jp/entry/2022/01/31/213300
 *   https://kazun-kyopro.hatenablog.com/entry/ABC/237/F
 * 
 * 二分探索でLISを求めるときの配列で考える。
 * つまりLIS={3,5,7}でval=4が来たときにLIS={3,4,7}に更新されるようなやつ。
 * DP[i][a][b][c]を以下のように定義する。
 *   i個目まで見て、 (i=1～N)
 *   LIS={a,b,c} であるような、 (1 <= a,b,c <= M)
 *   数列の個数
 *   ※LIS>=4となる数列はカウントしないことになる
 * 
 * 遷移は、i番目に値mがあったときに、LIS={a,b,c}のどこが変化するかを全探索する。
 * 例えば a<m<=b であればbの部分がmに置き換わるので、
 *   dp[i][a][m][c] += dp[i-1][a][b][c]
 * とすればよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・LISの気持ちを理解する。
 *   二分探索でLISを求めるときのLIS配列で、長さ3固定で考えて良い、という点に気付けるようにする。
 * ・制約を見て、状態数がN*M^3とか可能かな、と推測する。3はLIS=3という部分から。
 *   そこからDP値を逆算で考えてみたりする。
 */

ll dp[1001][12][12][12];

int main(void)
{
	int i, m, a, b, c;
	ll N, M; cin >> N >> M;
	// もらうDP
	dp[0][M+1][M+1][M+1] = 1;  // INFをM+1としている
	for(i = 1; i <= N; i++)
	{
		for(m = 1; m <= M; m++)
		{
			for(a = 1; a <= M+1; a++)
			{
				for(b = 1; b <= M+1; b++)
				{
					for(c = 1; c <= M+1; c++)
					{
						if(m <= a)  // [a,b,c]でaがmに置き換わる
						{
							dp[i][m][b][c] += dp[i-1][a][b][c];
							dp[i][m][b][c] %= MOD;
						}
						else if(m <= b)
						{
							dp[i][a][m][c] += dp[i-1][a][b][c];
							dp[i][a][m][c] %= MOD;
						}
						else if(m <= c)
						{
							dp[i][a][b][m] += dp[i-1][a][b][c];
							dp[i][a][b][m] %= MOD;
						}
						// c<m のケースはLISの長さが4になってしまうため、計算しない
					}
				}
			}
		}
	}

	// a<b<c(<=M)となるdp[N][a][b][c]の総和が答
	// …と考えたが、DP遷移でa<b<cを満たすよう更新していたので、気にする必要なかったかも
	ll ans = 0;
	for(a = 1; a <= M; a++)
	{
		for(b = a+1; b <= M; b++)
		{
			for(c = b+1; c <= M; c++)
			{
				ans += dp[N][a][b][c];
				ans %= MOD;
			}
		}
	}
	cout << ans << endl;

	return 0;
}
