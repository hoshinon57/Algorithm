#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC281 https://atcoder.jp/contests/abc281

/*
 * コンテスト中に解けず、解説を見て実装した。
 * 動的計画法で解く。もらうDP.
 * 
 * dp[i][j][k]を以下のように定義する。
 *   i個目まで見たときに、  (i:0～N)
 *   j個選んでいて、      (j:0～N or K)
 *   その和をDで割った余りがkであるような数の、  (k:0～D-1)
 *   最大値
 * 「そのような数は存在しない」場合、-1とする。
 * 
 * dp[i][j][k]への遷移は以下2パターンとなる。
 * (1)a[i]を選ばない場合
 *   j,kは同じで、iが1つ手前の要素からコピーする
 * (2)a[i]を選ぶ場合
 *   選ぶ数がj-1個で、
 *   そこまでの和(の余り)が k-a[i] である要素にa[i]を加えたものとなる(※)。
 *   (※)現状のdp[i][j][k]との最大値を取る
 * 
 * 答は、K個選んで、Dの倍数＝Dで割った余りが0, の要素になるので、
 * dp[N][K][0]となる。
 * 
 * 計算量はO(N^2*D).
 * （O(NKD)の実装でも問題なかったと思うが、実行時間制限には間に合いそうなので、あまり考えずに済む方にした）
 */
int main(void)
{
	int i, j, k;
	int N, K, D;
	cin >> N >> K >> D;
	vector<ll> a(N+1);
	for(i = 1; i <= N; i++) cin >> a[i];

	// dp[i][j][k]:
	// i個目まで見たときに、  (i:0～N)
	// j個選んでいて、      (j:0～N or K)
	// その和をDで割った余りがkであるような数の、  (k:0～D-1)
	// 最大値
	vector dp(N+1, vector(N+1, vector<ll>(D, -1)));  // -1:そのような数は存在しない、を意味する
	dp[0][0][0] = 0;  // 初期値

	// もらうDP
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= N; j++)
		{
			for(k = 0; k < D; k++)
			{
				// a[i]を選ばない場合
				// j,kは同じで、iが1つ手前の要素からコピー
				if(dp[i-1][j][k] != -1)
				{
					dp[i][j][k] = dp[i-1][j][k];
				}

				// a[i]を選ぶ場合
				// 選ぶ数がj-1個で、そこまでの和(の余り)がk-a[i]であるものから遷移する
				if(j-1 >= 0)
				{
					ll tmp = dp[i-1][j-1][(k+D-(a[i]%D)) %D];  // 減算＆剰余なので、Dを足しておく
					if(tmp != -1)  // 遷移元が「そのような要素は存在しない」であればスルー
					{
						dp[i][j][k] = max(dp[i][j][k], tmp + a[i]);
					}
				}
			}
		}
	}

	// K個選び、Dで割った余りが0のものが答となる
	cout << dp[N][K][0] << endl;

	return 0;
}
