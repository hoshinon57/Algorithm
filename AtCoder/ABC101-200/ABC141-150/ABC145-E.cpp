#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC145 https://atcoder.jp/contests/abc145

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i番目の料理までを食べ終える or i番目は注文スキップする したときに、  (i=1～N)
 *   j分かかっていたときの、
 *   美味しさ合計の最大値
 * 
 * 注文は(T-1)分までだが、食べ終えるのはT分以降でもよい、という点が悩ましい。
 * 以下のように置き換えた。
 *   ・料理を、食べるのにかかる時間Aで昇順ソートする。
 *     食べる料理の一覧が決まっているならば、Aが短い方から順に食べた方が良さそう、というイメージ。
 *   ・dp[i][T-1]から食べ始める場合、dp[i+1][T+x]ではなくdp[i+1][T]へと遷移させる。
 *     min(T-k+x, T)というイメージ。
 *     ここの実装のため、配るDPにて実装した。
 * 後は基本的なナップザック問題になる。
 * 
 * dp[N][T]が答。
 */

int main(void)
{
	int i, j;
	int N, T; cin >> N >> T;
	vector<pair<int,int>> food(N);
	for(i = 0; i < N; i++)
	{
		cin >> food[i].first >> food[i].second;
	}
	sort(food.begin(), food.end());  // かかる時間で昇順ソート

	vector<vector<int>> dp(N+1, vector<int>(T+1, 0));  // dp[N+1][T+1]
	// 配るDP
	for(i = 0; i < N; i++)  // 料理i
	{
		for(j = 0; j <= T; j++)  // j分経過
		{
			auto [a, b] = food[i];
			// 料理iを食べない
			dp[i+1][j] = max(dp[i+1][j], dp[i][j]);

			// 料理iを食べる
			if(j == T) continue;  // j=Tのときはタイムオーバーであり、料理iを食べる遷移はさせない
			int nxt = min(j+a, T);  // Tを超えたら、時刻Tに食べ終わるとする
			dp[i+1][nxt] = max(dp[i+1][nxt], dp[i][j] + b);
		}
	}
	cout << dp[N][T] << endl;

	return 0;
}
