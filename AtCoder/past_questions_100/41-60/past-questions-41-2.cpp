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
 * 以下URLを参考に、past-questions-41.cppの計算量改善版。
 *   https://emtubasa.hateblo.jp/entry/2018/08/08/205157
 * 　https://www.ioi-jp.org/joi/2012/2013-yo/2013-yo-t4/review/2013-yo-t4-review.html
 * 
 * その日選ぶ服は、服の候補のうち派手さが最小か最大の2通りのみを考えればよい。
 * よってdp[i][j]を以下のように定義する。
 * i日目まで終わったときに、  (i=0～D-1)
 *  i日目に j=0:派手さが最小の服, j=1:派手さが最大の服 を着たときの、 (j=0,1)
 *  これまでの「派手さの差の絶対値の合計」の最大値
 * 
 * 事前に、各気温ごとに着れる服のうち、派手さが最小のものと最大のものを計算しておく。
 * ⇒以下ソースコードではshowy_minmax[][]が該当する。
 * 
 * DPの遷移は、もらうDPにて以下のようになる。
 * dp[i][0] : 
 *   dp[i-1][0] + abs(minmax[t[i]][0] - minmax[t[i-1]][0])  昨日に派手さ最小の服を着るパターン
 *   dp[i-1][1] + abs(minmax[t[i]][0] - minmax[t[i-1]][1])  昨日に派手さ最大の服を着るパターン
 * のうち大きい方。
 * dp[i][1]も同様。
 * 
 * 計算量は、
 *  上記showy_minmaxの事前計算にてO(N), もしくは取りうる気温の範囲をTとしてO(NT).
 *  DP遷移にてO(D).
 * 合わせてO(NT+D) とか、かなあ。
 */

int dp[100][2] = {0};

int main(void)
{
	// 0-indexed
	int i, j, k;
	int D, N;
	cin >> D >> N;
	vector<int> t(D);  // T[d]:d日目の気温
	for(i = 0; i < D; i++) cin >> t[i];

	// showy_minmax[i][j]:
	//   気温iのときに着れる服のうち、
	//   j=0:派手さが最小のもの,j=1:最大のもの
	int showy_minmax[61][2] = {0};  // 気温は0～60度の計61パターン
	for(i = 0; i <= 60; i++)
		showy_minmax[i][0] = INF32;  // [0]側は最小値なので、無限大を設定しておく
	for(i = 0; i < N; i++)
	{
		int a, b, c;  // 着るのに適した気温(a,b)と、派手さ(c)
		cin >> a >> b >> c;
		for(j = a; j <= b; j++)
		{
			showy_minmax[j][0] = min(showy_minmax[j][0], c);  // ある気温jのときに着れる服のうち、派手さ最小値
			showy_minmax[j][1] = max(showy_minmax[j][1], c);  // 同じく派手さ最大値
		}
	}

	// dp[i][j]
	//   i日目まで終わったときに、  (i=0～D-1)
	//   i日目に j=0:派手さが最小の服, j=1:派手さが最大の服 を着たときの、 (j=0,1)
	//   これまでの「派手さの差の絶対値の合計」の最大値

	// dp[i][0] : 
	//   dp[i-1][0] + abs(minmax[t[i]][0] - minmax[t[i-1]][0])  昨日に派手さ最小の服を着るパターン
	//   dp[i-1][1] + abs(minmax[t[i]][0] - minmax[t[i-1]][1])  昨日に派手さ最大の服を着るパターン
	// のうち大きい方。
	// dp[i][1]も同様。

	// もらうDP
	for(i = 1; i < D; i++)
	{
		for(j = 0; j < 2; j++)  // 当日に着る服の派手さ j=0(最小),1(最大)
		{
			for(k = 0; k < 2; k++)  // 前日に来た服の派手さ k=0(最小),1(最大)
			{
				int tmp;
				tmp = dp[i-1][k] + abs(showy_minmax[t[i]][j] - showy_minmax[t[i-1]][k]);
				dp[i][j] = max(dp[i][j], tmp);
			}
		}
	}

	// 最終日に派手さが最小の服 or 最大の服を着るパターンのどちらか
	cout << max(dp[D-1][0], dp[D-1][1]) << endl;

	return 0;
}
