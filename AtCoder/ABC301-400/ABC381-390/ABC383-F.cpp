#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC383 https://atcoder.jp/contests/abc383

/*
 * 動的計画法で解く。
 * 
 * 単純に考えると、dp[i][j][k]として
 *   i個目まで見て、購入価格がjで、購入した色の種類数がk
 * だが、もちろん間に合わない。
 * 
 * ポイントとなるのが色の種類数。ここをどうにかできないか考える。
 * 商品をソートしても問題ないので、色でソートしておいて、
 *   1つ前の商品からの遷移    DP値は効用分だけ増加
 *   1つ前の「色」からの遷移  DP値は効用分＋K だけ増加
 * という遷移を考える。
 * すると次元が1つ減り、各状態ごとの遷移も2パターン、つまりO(1)であることから、全体でO(NX)で解ける。
 * 
 * 改めて、dp[i][j]を以下のように定義する。
 *   i個目の商品まで見て、 (i=1～N)
 *   購入した合計価格がjのときの、 (j=0～X)
 *   満足度
 * dp[N][0]～dp[N][X]の最大値が答。
 * 
 * [ACまでの思考の流れ]
 * ・ぱっと見でDPっぽい。
 * ・計算量はO(NX)かな…でも色の種類数という情報がジャマだなあ。
 *   前回と同じ色の商品を買ってもKは増えないので、前回と同じ色と違う色でそれぞれ考えてみる…か？
 */

int main(void)
{
	// 1-indexed
	ll i, j;
	ll N, X, K; cin >> N >> X >> K;
	vector<array<ll,3>> dt(N+1);  // {色C, 価格P, 効用U}の順に並べる
	dt[0][0] = -INF32;  // 色Cでソートするため
	for(i = 0; i < N; i++)
	{
		cin >> dt[i][1] >> dt[i][2] >> dt[i][0];
	}
	sort(dt.begin(), dt.end());  // 色でソート

	vector<vector<ll>> dp(N+1, vector<ll>(X+1, -INF64));  // dp[N+1][X+1]
	dp[0][0] = 0;
	ll prec_idx = 0;  // 直近ではpre->(pre+1)にて色が変わった
	for(i = 1; i <= N; i++)
	{
		if(dt[i][0] != dt[i-1][0]) prec_idx = i-1;  // 色が変わった
		for(j = 0; j <= X; j++)  // dp[i][j]
		{
			dp[i][j] = dp[i-1][j];  // 買わないケース

			// 商品iの価格: dt[i][1]
			if(j-dt[i][1] >= 0)
			{
				// 1つ前の商品(同じ色)からの遷移 (実際には違う色でも問題ない chmaxしているので)
				chmax(dp[i][j], dp[i-1][j-dt[i][1]] + dt[i][2]);

				// 前の色からの遷移
				chmax(dp[i][j], dp[prec_idx][j-dt[i][1]] + dt[i][2] + K);
			}
		}
	}

	ll ans = *max_element(dp[N].begin(), dp[N].end());
	cout << ans << endl;

	return 0;
}
