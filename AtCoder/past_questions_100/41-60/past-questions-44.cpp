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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題44
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1167&lang=jp

/*
 * 動的計画法(もらうDP)で解く。
 * 
 * まず、正四面体数、および奇数の正四面体数を事前計算しておく。
 * 求める和は最大10^6であるため、事前計算もこれを上限として良い。
 * これをそれぞれplk_1, plk_2とする。
 * 
 * 次にdp[i][j]を以下のように定義する。
 *   plk[i]まで使うときに、  (i=0～a.size()-1)
 *   和をjにできるような組み合わせの、  (j=0～10^6)
 *   足す個数の最小値
 * 
 * するとDPの遷移は、もらうDPにて以下のようになる。
 * (1)plk[i]を使わない場合
 *   dp[i][j] = min(dp[i][j], dp[i-1][j])
 * (2)plk[i]を使う場合
 *   同じplk[]を何度足しても良いので、
 *   dp[i][j] = min(dp[i][j], dp[i-1][j-plk[i]] + 1)
 *   dp[i][j] = min(dp[i][j], dp[i  ][j-plk[i]] + 1)  // 同じplk[]を足すケース
 *   の2通りとなる。
 *   ただし(1)の計算を行うことで、(2)の前者の計算は不要となる。
 * 
 * ★ただし、この手法ではメモリ制限にてエラーとなる。
 * dp[][]の要素数が 約180 * 10^6 = 約180MB ほどであり、本問題の上限128MBを超えるため。
 * そこでdp[i][j]のうち[i]の方を使わず、dp[j]のみでやりくりする実装とする。
 * またこの場合、DP遷移の(1)も計算不要となる。
 * 
 * 以上の方針でDPテーブルを作成したら、後はクエリごとにdp[n]を出力すればよい。
 * 
 * 計算量は、正四面体数の数をN, 和の上限をMとして、O(NM).
 * N=180程度、M=10^6であり、実行時間制限に間に合う。
 */

const int SUM_MAX = 1000000;

// DPテーブル作成
void solve(vector<int> &plk, vector<int> &dp)
{
	// もらうDP
	int i, j;
	for(i = 0; i < (int)plk.size(); i++)
	{
		for(j = 0; j <= SUM_MAX; j++)
		{
			// a[i]を使わない場合
			;  // 処理不要

			// a[i]を使う場合
			if(j-plk[i] >= 0)
				dp[j] = min(dp[j], dp[j-plk[i]] + 1);
		}
	}
}

int main(void)
{
	int i;
	vector<int> plk_1, plk_2;  // plk_2は奇数のみ使う用

	for(i = 0; i < 200; i++)  // i=185辺りで i*(i+1)*(i+2)/6 > 10^6 を満たすので、上限200程度で
	{
		int n = i*(i+1)*(i+2) / 6;
		if(n >= SUM_MAX) break;

		plk_1.push_back(n);
		if(n%2 == 1) plk_2.push_back(n);
	}

	// dp[i][j]
	//   plk[i]まで使うときに、  (i=0～a.size()-1)
	//   和をjにできるような組み合わせの、  (j=0～10^6)
	//   足す個数の最小値
	// ただしメモリ制限の関係で、dp[j]の1次元でまででやりくりする
	vector<int> dp_1(SUM_MAX+1, INF32);  // plk_1用
	vector<int> dp_2(SUM_MAX+1, INF32);  // plk_2用
	dp_1[0] = dp_2[0] = 0;  // DP初期値

	// DPテーブル作成
	solve(plk_1, dp_1);
	solve(plk_2, dp_2);

	while(true)
	{
		int n;
		cin >> n;
		if(n == 0) break;

		cout << dp_1[n] << " " << dp_2[n] << endl;
	}

	return 0;
}
