#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC162 https://atcoder.jp/contests/abc162

/*
 * 公式解説を元にした実装。
 *   https://img.atcoder.jp/abc162/editorial.pdf
 *   https://x.com/kyopro_friends/status/1249336461318447106
 *   https://betrue12.hateblo.jp/entry/2020/04/23/002522
 *   https://maspypy.com/atcoder-%E5%8F%82%E5%8A%A0%E6%84%9F%E6%83%B3-2020-04-12abc-162
 * 
 * dp[i][j]を以下のように定義する。
 *   i番目まで見て、  (i=1～N)
 *   それまでにj個選んだときの、 (j=0～N)
 *   総和の最大値
 * 単純なDPであり、このままだと状態数がO(N^2)で間に合わない。
 * ただし「連続して選ばない」という制約より、各iについて取れるjの範囲は
 *   (i-1)/2, (i+1)/2
 * の2通りであることが分かる。（実験などで）
 * 
 * jについてはmapを用いた。
 * 
 * DP遷移にて「連続して選ばない」を実現するために、
 * a[i]を使うケースにて dp[i-2][j-1] からの遷移とした。
 * （DP要素として「iを使ったか」を追加する方針でもいけると思う）
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・DPにて探索範囲を狭める、という考え方。
 * ・DPを単純に「i個目まで見て、j個選んだとき」とするとO(N^2)で間に合わない。
 *   ただし「連続して選ばない」という制約より、各iについて取れるjの範囲は狭いことが分かる。
 *   ここから探索範囲を狭めることを考える。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}  // 1-indexed

	vector<map<int,ll>> dp(N+1);
	for(i = 0; i <= 1; i++)
	{
		for(j = 0; j < 5; j++) dp[i][j] = -INF64;  // 適当に周辺を-INFで初期化
	}
	dp[0][0] = 0;
	dp[1][0] = 0;
	dp[1][1] = a[1];

	for(i = 2; i <= N; i++)
	{
		// 次回のiにおける dp[i-1] アクセスのために、周辺を-INFで初期化しておく (1WA)
		for(j = max((i-1)/2-2, 0); j <= min((i+1)/2+2, N); j++)
		{
			dp[i][j] = -INF64;
		}

		for(j = (i-1)/2; j <= (i+1)/2; j++)
		{
			// a[i]を使わない
			dp[i][j] = dp[i-1][j];

			// a[i]を使う
			if(j-1 >= 0)
			{
				chmax(dp[i][j], dp[i-2][j-1] + a[i]);  // 連続して選ばないので、i-1ではなくi-2から遷移
			}
		}
	}

	ll ans = dp[N][N/2];
	cout << ans << endl;

	return 0;
}
