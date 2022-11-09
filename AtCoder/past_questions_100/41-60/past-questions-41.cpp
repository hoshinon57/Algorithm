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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題41
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2013yo/tasks/joi2013yo_d

/*
 * 動的計画法で解く。
 *
 * dp[i][j]を以下のように定義する。
 *   i日目まで終わったときに、  (i=0～D-1)
 *   i日目に服jを着たときの、  (j=0～N-1)
 *   これまでの「派手さの差の絶対値の合計」の最大値
 *   ただし、服jを着れない気温の場合、dp[i][j]=-1とする
 * 
 * DPの遷移は、もらうDPにて
 *   dp[i][j] <= 各k(k=0～N-1)にて dp[i-1][k] + abs(c[j]-c[k]) の最大値
 * となる。
 * 
 * dp[D-1][0～N-1]のうちの最大値が求める答となる。
 * 計算量はO(DN^2).
 */

int dp[201][201] = {0};

int main(void)
{
	// 0-indexed
	int i, j, k;
	int D, N;
	cin >> D >> N;
	vector<int> t(D);  // T[d]:d日目の気温
	for(i = 0; i < D; i++) cin >> t[i];
	vector<int> a(N), b(N), c(N);  // 着るのに適した気温(a,b)と、派手さ(c)
	for(i = 0; i < N; i++) cin >> a[i] >> b[i] >> c[i];

	// dp[i][j]
	//   i日目まで終わったときに、  (i=0～D-1)
	//   i日目に服jを着たときの、  (j=0～N-1)
	//   これまでの「派手さの差の絶対値の合計」の最大値
	//   ただし、服jを着れない気温の場合、dp[i][j]=-1とする

	// 0日目の事前計算
	for(j = 0; j < N; j++)
	{
		if(t[0] < a[j] || t[0] > b[j])
			dp[0][j] = -1;  // 0日目に着れない服
	}

	// もらうDP
	for(i = 1; i < D; i++)  // i日目
	{
		for(j = 0; j < N; j++)  // 着る服j
		{
			if(t[i] < a[j] || t[i] > b[j])  // 服jが着れない場合
			{
				dp[i][j] = -1;
				continue;
			}

			int tmp = 0;
			for(k = 0; k < N; k++)  // 前日に来た服k
			{
				if(dp[i-1][k] == -1) continue;  // 前日、服kを着れない場合はスルー
				tmp = max(tmp, dp[i-1][k] + abs(c[j]-c[k]));
			}
			dp[i][j] = tmp;
		}
	}

	// dp[D-1][0～N-1]のうちの最大値
	int answer = 0;
	for(j = 0; j < N; j++)
	{
		answer = max(answer, dp[D-1][j]);
	}
	cout << answer << endl;

	return 0;
}
