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
 * 
 * dp[l][r]を以下のように定義する。
 *   [l,r)の区間の行列を計算した際の、スカラー乗算の回数の最小値
 *   l,r:0～N
 * 
 * DPの遷移は以下。
 * (1)r-l = 1のとき
 *   行列は1つであり、スカラー乗算は無いためdp[l][r]は0.
 * (2)r-l = 2のとき
 *   行列は2つであり、スカラー乗算の回数は lの列数*(lの行数*rの列数) となる。
 * (3)r-l >= 3のとき
 *   例えば =3 のとき、「行列1つと行列2つ」「行列2つと行列1つ」でそれぞれ計算して
 *   スカラー乗算回数が少ない方を採用する。
 *   行列を区分けする変数(border)を用意したときに、
 *     dp[l][border] + dp[border][r] + (borderの手前の行列の行数)*(lの行数*rの列数)
 *   がスカラー乗算回数となるので、
 *   全てのborderの選び方についてそれぞれ計算し、その最小値がdp[l][r]となる。
 *   これは再帰で実装した。
 * 
 * 以上を実装したときに、dp[0][N]が答となる。
 */

int N;
vector<pair<int,int>> M;  // M個の行列を表す pairは{Row,Column}の順

// dp[l][r]
//   [l,r)の区間の行列を計算した際の、スカラー乗算の回数の最小値
//   l,r:0～N
// -1なら未計算
vector<vector<int>> dp;

// dp[l][r]をメモ化再帰で解く
int func(int l, int r)
{
	if(dp[l][r] != -1) return dp[l][r];  // メモ化済み
	
	if(r-l == 1)  // 行列1つ
	{
		dp[l][r] = 0;  // スカラー乗算回数は0
		return dp[l][r];
	}
	if(r-l == 2)  // 行列2つ
	{
		// lの列数*(lの行数*rの列数) がスカラー乗算回数になる
		dp[l][r] = M[l].second * (M[l].first * M[r-1].second);
		return dp[l][r];
	}

	int ret = INF32;
	for(int border = l+1; border <= r-1; border++)
	{
		int tmp;
		tmp = func(l, border) + func(border, r);  // 右辺はそれぞれ dp[l][border], dp[border][r] を指す
		tmp += M[border].first * (M[l].first * M[r-1].second);
		ret = min(ret, tmp);
	}
	dp[l][r] = ret;
	return dp[l][r];
}

int main(void)
{
	int i;
	cin >> N;
	M.assign(N, {0,0});
	for(i = 0; i < N; i++)
	{
		cin >> M[i].first >> M[i].second;
	}
	dp.assign(N+1, vector<int>(N+1, -1));  // 添字が0～Nの範囲であるためN+1

	cout << func(0, N) << endl;

	return 0;
}
