#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題46
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_10_B&lang=ja

/*
 * 区間DPで解く。
 * 別解法での練習を目的として、再帰を使わず実装した。
 * 
 * DPの定義は同じく以下。
 *   [l,r)の区間の行列を計算した際の、スカラー乗算の回数の最小値
 *   l,r:0～N
 * 
 * DPの遷移も同様(省略)。
 * 再帰を使わないので、dp[l][r]について、r-l(=width)が小さいものから順に計算していく。
 * dp[0][N]が答となるのも同様。
 * 
 * 計算量は変わらずO(N^3).
 */

int N;
vector<pair<int,int>> M;  // M個の行列を表す pairは{Row,Column}の順

// dp[l][r]
//   [l,r)の区間の行列を計算した際の、スカラー乗算の回数の最小値
//   l,r:0～N
vector<vector<int>> dp;

int main(void)
{
	cin >> N;
	M.assign(N, {0,0});
	for(int i = 0; i < N; i++)
	{
		cin >> M[i].first >> M[i].second;
	}
	dp.assign(N+1, vector<int>(N+1, 0));  // 添字が0～Nの範囲であるためN+1

	int width;
	int l, r;
	// dp[l][r]について、r-l(=width)が小さいものから順に計算していく
	for(width = 1; width <= N; width++)
	{
		for(l = 0; l < N; l++)
		{
			// [l,r)を計算
			r = l+width;
			if(r > N) break;  // 範囲外

			if(width == 1) dp[l][r] = 0;
			else if(width == 2)
			{
				dp[l][r] = M[l].second * (M[l].first * M[r-1].second);
			}
			else
			{
				int ret = INF32;
				for(int border = l+1; border <= r-1; border++)
				{
					int tmp;
					tmp = dp[l][border] + dp[border][r];
					tmp += M[border].first * (M[l].first * M[r-1].second);
					ret = min(ret, tmp);
				}
				dp[l][r] = ret;
			}
		}
	}

	cout << dp[0][N] << endl;

	return 0;
}
