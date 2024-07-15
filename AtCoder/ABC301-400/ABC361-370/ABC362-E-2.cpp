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

// ABC362 https://atcoder.jp/contests/abc362

/*
 * 公式解説におけるO(N^4)解法とほぼ同じ方針と思われる。
 *   https://atcoder.jp/contests/abc362/editorial/10399
 * 
 * dp[i][j][l]を以下のように定義する。
 *   最後にAi, 1つ前にAjを選び、 (i,j=1～N, j<i)
 *   長さがlの、 (l>=2)
 *   等差数列の個数
 * 
 * 後は実装を参照。
 * まずはlを固定し、次にi,jを固定し、i+1番目以降の要素Akを探索していった。
 * Ak-Ai=Ai-AjならばAj,Ai,Akの順に等差数列となるので、
 * dp[k][i][l]にdp[i][j][l-1]を加算すればよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・DPって結局のところ「直近で必要な要素のみを保持し、不要な古い要素は忘れる」という考え方。
 *   また等差数列は、直近2つの要素が分かれば次の要素が分かる。
 *   よってDPとして直近とその1つ前に選ぶ要素を保持する、という思考でたどり着ける。(今回は長さも必要)
 */

int main(void)
{
	// 1-indexed
	int i, j, k, l;
	ll N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}

	vector dp(N+1, vector(N+1, vector<ll>(N+1, 0)));
	for(l = 2; l <= N; l++)
	{
		for(i = 1; i <= N; i++)
		{
			for(j = 1; j < i; j++)  // j<i
			{
				if(l == 2)
				{
					dp[i][j][l] = 1;  // DP初期値
					continue;
				}
				for(k = i+1; k <= N; k++)
				{
					if(a[k]-a[i] == a[i]-a[j])  // Aj,Ai,Akの順に等差数列
					{
						dp[k][i][l] += dp[i][j][l-1];
						dp[k][i][l] %= MOD;
					}
				}
			}
		}
	}

	cout << N;  // k=1 このケースだけ特別に出力
	for(k = 2; k <= N; k++)
	{
		// dp[*][*][k]の総和
		ll ans = 0;
		for(i = 1; i <= N; i++)
		{
			for(j = 1; j <= N; j++)
			{
				ans += dp[i][j][k];
				ans %= MOD;
			}
		}
		cout << " " << ans;
	}
	cout << endl;

	return 0;
}
