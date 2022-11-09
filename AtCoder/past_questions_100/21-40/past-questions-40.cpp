#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int MOD = 10000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題40
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2012yo/tasks/joi2012yo_d

/*
 * 動的計画法で解く。
 *
 * dp[i][j][k]を以下のように定義する。
 *   i日目まで終わったときに、  (i:0～N-1)
 *   当日のパスタがjで、       (j:0～2)
 *   前日のパスタがkのときの、  (k:0～2)
 *   組み合わせの数
 * 
 * DPの遷移は、もらうDPにて
 *   dp[当日][j][k] <- dp[前日][k][h:0～2]を加算
 * となる。
 * 「3日以上同じパスタはNG」という条件より、j=k=hのケースは除外する。
 * また、特定の日のパスタをすでに決めている場合は、dp[i][j][k]にて対象外のjは除外して計算する。
 * 
 * dp[N-1][0～2][0～2]の総和が求める答となる。
 * 計算量は、パスタの種類をM(M=3)として、O(NM^3).
 * 
 * 【補足】
 * 「3日連続」の条件に対して、1日前ないし2日前が存在しないケースがあるのが厄介。
 * 自分は1日目の事前設定にて「当日/前日で別のところにカウントを振り分ける」としたが、色々な手法がある。
 * 参考：
 *   https://betrue12.hateblo.jp/entry/2020/03/29/232441
 *   https://kakedashi-engineer.appspot.com/2020/06/09/joi2012yod/
 */

int dp[100][3][3] = {0};

int main(void)
{
	// 0-indexed
	int i, j, k, h;
	int N, K;
	cin >> N >> K;
	vector<int> decide(N, -1);  // decide[i]:i日目に既に決めているパスタの番号(0～2)  未定なら-1
	for(i = 0; i < K; i++)
	{
		int a, b;
		cin >> a >> b;
		a--;  // 0-indexedに変換
		b--;
		decide[a] = b;
	}

	// dp[i][j][k]
	//   i日目まで終わったときに、  (i:0～N-1)
	//   当日のパスタがjで、  (j:0～2)
	//   前日のパスタがkのときの、  (k:0～2)
	//   組み合わせの数

	// 1日目の事前計算
	for(j = 0; j < 3; j++)
	{
		if(decide[0] != -1 && decide[0] != j) continue;  // 1日目のパスタを決定済み
		for(k = 0; k < 3; k++)
		{
			if(j==k) dp[0][j][(j+1)%3] = 1;  // とりあえず、当日/前日で別のところにカウントを振り分けておく
		}
	}

// 2日目の事前計算を特出しする必要は無い。
// 1日目の事前計算にて当日/前日で別のところにカウントを振り分けており、2日目の計算でj=k=hの条件に引っかかることは無いため。
#if 0  
	// 2日目の事前計算
	for(j = 0; j < 3; j++)
	{
		if(decide[1] != -1 && decide[1] != j) continue;  // 2日目のパスタを決定済み
		for(k = 0; k < 3; k++)
		{
			for(h = 0; h < 3; h++)
			{
				dp[1][j][k] += dp[0][k][h];
			}
		}
	}
#endif

	// もらうDP
	for(i = 1; i < N; i++)  // i日目
	{
		for(j = 0; j < 3; j++)  // 当日のパスタの種類
		{
			if(decide[i] != -1 && decide[i] != j) continue;  // 本日のパスタはdecide[i]にて決定済み

			for(k = 0; k < 3; k++)  // 前日のパスタの種類
			{
				for(h = 0; h < 3; h++)  // 前々日のパスタの種類
				{
					if(j == k && k == h) continue;  // 3日連続はNG

					// dp[当日][j][k] <- dp[前日][k][h]を加算
					dp[i][j][k] += dp[i-1][k][h];
					dp[i][j][k] %= MOD;
				}
			}
		}
	}

	// dp[N-1][0～2][0～2]の総和
	int answer = 0;
	for(j = 0; j < 3; j++)
	{
		for(k = 0; k < 3; k++)
		{
			answer += dp[N-1][j][k];
			answer %= MOD;
		}
	}
	cout << answer << endl;

	return 0;
}
