#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題43
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/pakencamp-2019-day3/tasks/pakencamp_2019_day3_d

/*
 * 動的計画法で解く。(もらうDP)
 * 
 * dp[i][j]を以下のように定義する。
 *   i列目まで見ていて、  (i=1～N)
 *   i列目を色jで塗ったときの、  (j=0:赤, 1:青, 2:白)
 *   塗るマスの最小値
 * 
 * するとDPの遷移は以下のようになる。
 * dp[i][j] = min(dp[i-1][0～2 ただし≠j]) + i列目を色jにする場合の塗るマス数
 * 
 * 出力はdp[N][0～2]のうちの最小値となる。
 * 
 * 計算量はO(N). (行数の5や、色の種類の3は無視する)
 */

int main(void)
{
	// 0-indexed
	int i, j, k;
	int N;
	cin >> N;
	vector<vector<int>> a(N+1, vector<int>(5));  // a[i][j]:S[j][i] i:1～N, j:0～4
	for(i = 0; i < 5; i++)
	{
		string str;
		cin >> str;
		map<char, int> m;
		m['R'] = 0;
		m['B'] = 1;
		m['W'] = 2;
		m['#'] = 3;
		for(j = 1; j <= N; j++)
		{
			a[j][i] = m[str[j-1]];
		}
	}

	// dp[i][j]
	// i列目まで見ていて、  (i=1～N)
	// i列目を色jで塗ったときの、  (j=0:赤, 1:青, 2:白)
	// 塗るマスの最小値
	vector<vector<int>> dp(N+1, vector<int>(3, INF32));
	for(i = 0; i < 3; i++) dp[0][i] = 0;  // DP初期値

	// もらうDP
	for(i = 1; i <= N; i++)  // i列目
	{
		for(j = 0; j < 3; j++)  // 色jで塗る
		{
			// i列目を色jで塗る場合、何マス塗ればよいか
			int num = 0;
			for(k = 0; k < 5; k++)
			{
				if(a[i][k] != j) num++;  // 入力データと塗りたい色が異なる場合
			}

			for(k = 0; k < 3; k++)  // k:前の列i-1を色kで塗った場合、からの遷移を考える
			{
				if(j == k) continue;  // 2列を同じ色では塗れない
				dp[i][j] = min(dp[i][j], dp[i-1][k]+num);
			}
		}
	}

	// dp[N][*]の最小値が答
	int answer = INF32;
	for(k = 0; k < 3; k++)
	{
		answer = min(answer, dp[N][k]);
	}
	cout << answer << endl;

	return 0;
}
