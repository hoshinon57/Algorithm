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
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * いやー難しかった、解説を見て方針を理解しても、実装に死ぬほど苦労した…
 * 精進を続けて、将来見直したらすんなり解けるようになってるかな…
 *   https://kyopro-friends.hatenablog.com/entry/2019/01/12/231035
 *   https://scrapbox.io/pocala-kyopro/T_-_Permutation
 *   https://blog.hamayanhamayan.com/entry/2019/01/12/150046
 *   https://qiita.com/Series_205/items/7d2c57b45179006d0bc6
 * 
 * 基本は各種解説の通りで、
 * dp[i][j]を以下のように定義する。
 *   i個目の要素まで見て、 (i=1～N)
 *   i個目の値よりも小さい要素がj個残っている  (j=0～N-1)
 *   ときの、組み合わせの総数
 * ※状態数を削減する件は記載割愛。
 * 
 * dp[N][0]が答。
 * N番目まで並べたら未使用の要素は無いためj=0.
 * 
 * dp[i][j]へのDP遷移は、もらうDPにて
 *   ">"のとき：dp[i-1][j+1]～dp[i-1][N-1] の和
 *   "<"のとき：dp[i-1][0]～dp[i-1][j]の和
 *              ただしj<=N-iのときのみ遷移可能。j>N-iだと「要素がj個残っている」のがありえないため0.
 * となる。
 * これを単純に実装すると状態数がN^2, 遷移がNのため計算量がO(N^3)となりTLE.
 * 累積和を使って遷移をO(1)にすることで、O(N^2)を実現する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・右端の値をDP配列として持たずとも、「それより小さい数がj個」という情報だけで実現できる。
 *   不要な情報は存在しないか、を意識する。
 * ・DP遷移を累積和で高速化するという考え方。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	string s; cin >> s;
	vector<vector<ll>> dp(N+1, vector<ll>(N, 0));
	for(i = 0; i <= N-1; i++) dp[1][i] = 1;  // DP初期値

	for(i = 2; i <= N; i++)
	{
		vector<ll> sum(N+1,0);
		for(j = 0; j < N; j++)
		{
			sum[j+1] = sum[j] + dp[i-1][j];
		}
		for(j = 0; j < N; j++)
		{
//			int k;
			if(s[i-2] == '>')
			{
//				for(k = j+1; k < N; k++) dp[i][j] += dp[i-1][k];
				dp[i][j] = sum[N] - sum[j+1];
			}
			else if(j <= N-i)
			{
//				for(k = 0; k <= j; k++) dp[i][j] += dp[i-1][k];
				dp[i][j] = sum[j+1]- sum[0];
			}
			dp[i][j] %= MOD;
		}
	}

	cout << dp[N][0] << endl;

	return 0;
}
