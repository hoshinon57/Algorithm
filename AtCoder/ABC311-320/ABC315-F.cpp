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

// ABC315 https://atcoder.jp/contests/abc315

/*
 * 動的計画法で解く。
 * 
 * まず、全チェックポイントを通ったときの総移動距離が、最大でどれぐらいかを計算すると、
 *   (10^4 * √2) * 10^4 = 141,000,000 < 10^9 ≒ 2^30
 * となる。
 * よってペナルティの計算方法より、チェックポイントを30個以上無視するのはありえない戦略とみなせる。
 * 
 * ここでdp[i][j]を以下のように定義する。
 *   ポイントiまで見て、  (i=0～N-1)
 *   チェックポイントをj個省略したときの、  (j=0～30 で枝刈り)
 *   総移動距離
 * ペナルティは最後に計算し、dp[]には含めない。
 * 
 * DP遷移は、もらうDPにて
 *   k:地点iの前ポイントからiへと移動する際に、無視するチェックポイントの数 として
 *   dp[i][j] = dp[i-1-k][j-k] + dist(i, i-1-k)
 *   の各kにおける最小値
 * となる。
 * 
 * 計算量は、チェックポイントを無視する最大数をCとして、O(N*C^2)となる。
 */

int x[10005], y[10005];
double dp[10005][35] = {0.0};

int main(void)
{
	// 0-indexed
	int i, j, k;
	int N; cin >> N;
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
	}

	// ポイントp1とp2の距離
	auto dist = [&](int p1, int p2) -> double
	{
		ll dx = x[p1] - x[p2];
		ll dy = y[p1] - y[p2];
		return sqrt(dx*dx+dy*dy);
	};

	// もらうDP
	for(i = 1; i < N; i++)
	{
		for(j = 0; j <= 30; j++)
		{
			// dp[i][j]を計算
			dp[i][j] = INF64;
			for(k = 0; k <= 30; k++)  // 前ポイントからポイントiへの移動にあたり、k個省略する
			{
				int nxt = i-1-k;
				if(nxt >= 0 && j-k >= 0)
				{
					dp[i][j] = min(dp[i][j], dp[nxt][j-k] + dist(i, nxt));
				}
			}
		}
	}

	double ans = INF64;
	for(j = 0; j <= 30; j++)
	{
		ll pena = 0;
		if(j > 0) pena = 1<<(j-1);  // ペナルティ
		ans = min(ans, dp[N-1][j] + (double)pena);
	}
	cout << std::fixed << std::setprecision(20);
	cout << ans << endl;

	return 0;
}
