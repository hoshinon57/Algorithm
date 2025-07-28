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

// 後に実装したABC275-F-2.cppの方がやりやすい。

/*
 * ナップザックDPにて、Aを先頭から見ていったときに「これまでに削除操作が何回必要か？」と、
 * 次Aを選ぶ際に削除操作の要不要を知るために、「最後に選んだAは何番目か？」の情報が欲しい。
 * ※DPとしてどう値を持たせるかを考えたら出てきた
 * 
 * dp[i][j]を以下のように定義する。
 *   Aのi個目まで見て、  (i=0～N)
 *   選んだAの総和がjであるとき、  (j=0～M)
 *   「これまでに削除する範囲が確定した区間(※)」の個数と、使うAの最右の要素番号 のペア
 *   (※)削除範囲の左端と右端に、Aを選ぶ要素が存在するという意図。
 *       そのためAを選んだ際に、直近の削除範囲が確定する。
 * 
 * 初期値はdp[0][0]={0,0}. これは番兵的な意味も持つ。
 * 答は各dp[N][*]において、以下の条件を入れつつdp.firstとなる。
 *   dp.first=INF なら-1
 *   dp.secont!=Nなら最後の削除範囲が未確定のままなので、dp.firstに+1
 * 
 * DP遷移は(1)Aを使わないケースと(2)使うケースを比較する。
 * 使うケースでは、手前の要素も選んでいれば、追加の削除は無し。
 * 手前の要素を選んでいなければ、削除回数は+1となる。
 * 
 * (1)(2)の比較では、
 *   削除回数が小さい方が良い
 *   削除回数が同じなら、使うAの最右要素がなるべく大きい方が良い (今回選べば、次回選ぶ分には削除回数が増えないため)
 * となる。
 * 
 * 以下のkyopro_friends氏の方針、
 * および実装面では以下のscrapboxの方が良さそう。
 * https://x.com/kyopro_friends/status/1586355885731123200
 * https://scrapbox.io/kani/ABC275_-_F_-_Erase_Subarrays
 */

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	using pii = pair<int,int>;
	vector<vector<pii>> dp(N+1, vector<pii>(M+1, {INF32, 0}));  // dp[N+1][M+1]
	dp[0][0] = {0, 0};  // DP初期値、および番兵の役割にもなる

	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= M; j++)  // a[i][j]
		{
			pii t1 = dp[i-1][j];  // 使わないケース
			pii t2 = {INF32, 0};  // 使うケース
			if(j-a[i] >= 0)
			{
				t2 = dp[i-1][j-a[i]];
				if(t2.second == i-1)  // 前のAも選んでいるので、追加の切りは無い
				{
					t2 = {t2.first, i};
				}
				else
				{
					t2 = {t2.first+1, i};
				}
			}
			if(t1.first != t2.first)
			{
				dp[i][j] = min(t1, t2);  // firstが小さい方を選ぶ
			}
			else
			{
				dp[i][j] = max(t1, t2);  // firstが同じなので、secondが大きい方を選ぶ
			}
		}
	}

	for(j = 1; j <= M; j++)
	{
		pii d = dp[N][j];
		int ans = d.first;
		if(ans == INF32) ans = -1;
		else if(d.second != N) ans++;
		cout << ans << endl;
	}

	return 0;
}
