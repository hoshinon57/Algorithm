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

// ABC314 https://atcoder.jp/contests/abc314

/*
 * 期待値DPで解く。
 * 
 * dp[i]を以下のように定義する。
 *   必要なポイントが残りiポイントのとき、支払う金額の期待値
 * DP初期値はdp[0]=0, 答はdp[M]となる。
 * 
 * DPの遷移は以下のように考える。
 *   残iポイント必要で、ルーレットjについて考えるとき、
 *   dp[i] = ルーレットjの金額 + Σ(dp[i-ルーレットjの出目]/ルーレットjの出目数)
 * ただし出目に0があると右辺にもdp[i]が登場し、自己ループになってしまう。
 * そのため出目が0である個数をzeroとすると、
 *   dp[i] = ルーレットjの金額 + Σ(dp[i-ルーレットjの出目、ただし0以外]/ルーレットjの出目数) + dp[i]*zero/ルーレットjの出目数
 *   ⇒dp[i] * (1-zero/ルーレットjの出目数) = ルーレットjの金額 + Σ(dp[i-ルーレットjの出目、ただし0以外]/ルーレットjの出目数)
 *   ⇒dp[i] * (ルーレットjの出目数-zero)/ルーレットjの出目数 = ルーレットjの金額 + Σ(dp[i-ルーレットjの出目、ただし0以外]/ルーレットjの出目数)
 *   ⇒dp[i] = (ルーレットjの金額 + Σ(dp[i-ルーレットjの出目、ただし0以外]/ルーレットjの出目数)) * ルーレットjの出目数 / (ルーレットjの出目数-zero)
 * と、右辺からdp[i]を消すことができた。
 * (上記は見づらくなってしまったが、適当に紙に式を書くと理解しやすい)
 * 
 * ※もしくは、出目が0なら再度ルーレットしなおすという考え方で、
 *     ルーレットjの金額 * ルーレットjの出目数 / (ルーレットjの出目数 - zero)
 *   をルーレットjのの金額として置き換えてしまうやり方もある。
 *   (公式解説より)
 * 
 * 上記はルーレットjについての計算であるので、
 * これを全ルーレットについて計算し、その最小値がdp[i]となる。
 * 
 * 計算量はO(M*ΣP).
 */

int main(void)
{
	int i, j, k;
	int N, M; cin >> N >> M;
	vector<int> cost(N);  // cost[i] ルーレットiの金額
	vector<vector<int>> pt(N);  // pt[i][*] ルーレットiのポイント一覧
	vector<int> pt_size(N);  // pt_size[i] ルーレットiのポイントの種類数
	for(i = 0; i < N; i++)
	{
		cin >> cost[i];
		cin >> pt_size[i];
		for(j = 0; j < pt_size[i]; j++)
		{
			int s; cin >> s;
			pt[i].push_back(s);
		}
	}

	vector<double> dp(M+1, INF64);
	dp[0] = 0.0;  // DP初期値 Mポイント以上獲得したため、もうルーレットを回す必要はない
	for(i = 1; i <= M; i++)  // 残iポイント
	{
		for(j = 0; j < N; j++)  // ルーレットj
		{
			double d = cost[j];
			int zero = 0;  // ルーレットjのうち、0ポイントの個数
			for(k = 0; k < pt_size[j]; k++)
			{
				if(pt[j][k] == 0)
				{
					zero++;
					continue;
				}
				int nxt = max(i - pt[j][k], 0);
				d += dp[nxt] / pt_size[j];

			}
			d = d * pt_size[j] / (pt_size[j] - zero);
			dp[i] = min(dp[i], d);
		}
	}
	cout << std::fixed << std::setprecision(20);
	cout << dp[M] << endl;

	return 0;
}
