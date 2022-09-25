#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC270 https://atcoder.jp/contests/abc270

/*
 * 動的計画法で解く。
 * 類題：DPまとめコンテスト K問題
 *   https://atcoder.jp/contests/dp/tasks/dp_k
 * 
 * dp[i][j]を以下のように定める。
 *   i:残りの石がi個で、  (i=0～N)
 *   j:j=0なら自分(高橋君)のターン、j=1なら相手(青木君)のターンのときに、  (j=0,1)
 *   自分が取れる石の最大数
 * 
 * 遷移式は以下となる。
 * ・j=0、つまり自分のターンのとき、
 *   dp[i-Ak][1]+Ak (Ak:A1～AK)  の最大値。
 *   自分がA1～AKの中から選んだ個数を取って相手ターンになるので、
 *   「石がXX個で相手ターンのときに、自分は何個取れるか」＋「自分がAk個取る」の最大値、という考え方。
 * ・j=1、つまり相手ターンのとき、
 *   dp[i-Ak][0] (Ak:A1～AK)  の最小値。
 *   j=0との違いは、
 *     石を取るのは相手なので、Akの加算はしない
 *     お互いに最適行動をするので、dp[][]の値が小さくなるように行動する、つまり最小値を選ぶ
 *   という2点。
 * 
 * あとは上記を実装すればよい。
 * 出力するのは、先攻は自分であることから dp[N][0] となる。
 * 
 * [メモ、考え方]
 *   ゲームの問題は、後ろから求めていく
 */

int dp[10001][2] = {0};

int main(void)
{
	// 0-indexed
	int i, j, k;
	int N, K;
	cin >> N >> K;
	vector<int> a(K);
	for(i = 0; i < K; i++)
	{
		cin >> a[i];
	}

	// 最初、dp[][]の値は全て0
	for(i = 0; i <= N; i++)  // 残りの石が少ない方から埋めていく
	{
		for(j = 0; j < 2; j++)
		{
			for(k = 0; k < K; k++)
			{
				if(i-a[k] < 0) continue;
				if(j == 0)  // 自分のターン
				{
					dp[i][j] = max(dp[i-a[k]][1]+a[k], dp[i][j]);
				}
				else  // 相手のターン
				{
					// dp[i][j]へ設定するにあたり、dp[i][j]は初期状態で0になっている。
					// minを使うため、初期値0が採用されないよう、dp[i][j]=0なら無条件で更新する。
					if(dp[i][j] == 0)
					{
						dp[i][j] = dp[i-a[k]][0];
					}
					else
					{
						dp[i][j] = min(dp[i-a[k]][0], dp[i][j]);
					}
				}
			}
		}
	}

#if 0  // 結果確認用
	for(i = 0; i < 2; i++)
	{
		cout << i<<":";
		for(j = 0; j <= N; j++)
		{
			cout << std::setw(4);
			cout << dp[j][i];
		}
		cout << endl;
	}
#endif
	cout << dp[N][0] << endl;

	return 0;
}
