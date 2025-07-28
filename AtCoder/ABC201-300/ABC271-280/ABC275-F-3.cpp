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
 * kanpurin氏の解説を元に実装した内容。
 *   https://kanpurin.hatenablog.com/entry/2022/12/13/134224
 * 
 * dp[i][j]を以下のように定義する。
 *   i個目まで見て、 (i=0～N)  1-indexed
 *   和がjのときの、
 *   操作回数の最小値
 * 
 * a[i]を使わないケースにて、a[k]～a[i]まで削除すると考えると、k=[1,i]の範囲であり、
 *   min(dp[k-1][j])+1 がdp[i][j]となる。
 * 愚直にやるとTLEなので高速化を考える。
 * forループの順序をj,iの順にしてみると、min(dp[k-1][j])はiのループを回しながらO(1)で決められる。
 * 
 * min部分の最小値を管理する変数miについて、初期値に注意。
 * 基本はINFだが、j=0のときはmi=0を初期値にする必要がある。
 * 
 * ※実装ではdp[i]部分は1-indexed, a[i]は0-indexedになっている点に注意。
**/

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<vector<int>> dp(N+1, vector<int>(M+1, INF32));  // dp[N][M+1]
	dp[0][0] = 0;
	for(j = 0; j <= M; j++)
	{
		int mi = INF32;
		if(j == 0) mi = 0;  // DP初期値的な miの更新タイミングにより、dp[0][*]はmiに入らないため
		for(i = 1; i <= N; i++)
		{
#if 0  // 愚直処理
			// aを使うケース
			if(j-a[i-1] >= 0) dp[i][j] = dp[i-1][j-a[i-1]];  // dpのi部分とa[i]とでは添字が1つずれる

			// aを使わないケース
			for(k = 1; k <= i; k++)  // a[k]～a[i]を消す
			{
				chmin(dp[i][j], dp[k-1][j]+1);
			}
#else
			// aを使わないケース
			chmin(dp[i][j], mi+1);

			// aを使うケース
			if(j-a[i-1] >= 0)
			{
				chmin(dp[i][j], dp[i-1][j-a[i-1]]);  // dpのi部分とa[i]とでは添字が1つずれる
			}

			chmin(mi, dp[i][j]);
#endif
		}
	}

	for(j = 1; j <= M; j++)
	{
		int ans = dp[N][j];
		if(ans == INF32) ans = -1;
		cout << ans << endl;
	}

	return 0;
}
