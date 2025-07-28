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

// ABC275 https://atcoder.jp/contests/abc275

/*
 * 以下の解説を参考に実装した内容。
 *   https://x.com/kyopro_friends/status/1586355885731123200
 *   https://scrapbox.io/kani/ABC275_-_F_-_Erase_Subarrays
 * 
 * DPにて「いま見ている要素を削除中か？」の状態を持たせる。
 * 耳DPみたいな形か。
 * 
 * dp[j][f]を以下のように定義する。
 *   その要素まで見て、
 *   和がjで、 (j=0～M)
 *   f=0:a[i]を削除する、f=1:a[i]を削除しない
 *   としたときの、削除回数の最小値
 * 各問題についてdp[x][M]が答。
 * 
 * DP遷移は、f=0のa[i]を削除するとき、
 *   和jは変化しない
 *   前回のfが削除中なら、今回削除しても操作回数は増えない
 * などをふまえ、 ndp[j][0] = min(dp[j][0], dp[j][1]+1) となる。
 * 
 * f=1のa[i]を削除しないとき、
 *   和jはa[i]だけ増える
 *   fによらず操作回数は増えない
 * などをふまえ、 ndp[j][0] = min(dp[j-a[i]][0], dp[j-a[i]][1]) となる。
**/

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<vector<int>> dp(M+1, vector<int>(2, INF32));  // dp[M+1][2]
	dp[0][1] = 0;
	for(i = 0; i < N; i++)
	{
		vector<vector<int>> ndp(M+1, vector<int>(2, INF32));  // ndp[M+1][2]
		for(j = 0; j < M+1; j++)
		{
			// f=0: a[i]を削除するとき
			// 和jは変化しない
			ndp[j][0] = min(dp[j][0], dp[j][1]+1);  // dp[j][0],前回削除していたら、今回削除しても操作回数は増えない

			// f=1: a[i]を使うとき
			// 和はa[i]だけ増えるので、前回の和は(j-a[i])
			if(j-a[i] >= 0) ndp[j][1] = min(dp[j-a[i]][0], dp[j-a[i]][1]);
		}
		swap(dp, ndp);
	}

	for(j = 1; j < M+1; j++)
	{
		int ans = min(dp[j][0], dp[j][1]);
		if(ans == INF32) ans = -1;
		cout << ans << endl;
	}

	return 0;
}
