#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題45
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2199&lang=jp

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定める。
 *   i番目の入力信号まで見たときに、  (i:1～N)
 *   そのときの「複合化後の出力信号」がj, つまりy(j)であるときの、  (j:0～255)
 *   「差の二乗の和」の最小値
 * 
 * DP遷移は、配るDPを用いて、
 * dp[i][*] <-- dp[i-1][j] への遷移を考える。
 *   diff:差の二乗、つまりj+code[]とinput_data[i]の差の二乗
 * として、コードブックの各値について
 *   dp[i][j+code[*]] = min(dp[i-1][j] + diff);
 * となる。
 * 
 * dp[N][*] の最小値が答となる。
 * 
 * 計算量はO(NM).
 * 出力信号の値も含めて考えると、値の範囲をK(=0～255なので、K=256)として、O(NMK)となる。
 * ※N*M*K=およそ10^8
 */

const int OUTPUT_MAX = 255;  // 出力信号の最大値

int main(void)
{
	int i, j;

	while(true)
	{
		int N, M;
		cin >> N >> M;
		if(N == 0 && M == 0) break;

		vector<int> code(M);  // コードブックの値
		for(i = 0; i < M; i++)
		{
			cin >> code[i];
		}
		vector<int> input_data(N+1);  // 入力信号 (1～N)
		for(i = 1; i <= N; i++)
		{
			cin >> input_data[i];
		}

		// dp[i][j]
		//   i番目の入力信号まで見たときに、  (i:1～N)
		//   そのときの「複合化後の出力信号」がj, つまりy(j)であるときの、  (j:0～255)
		//   「差の二乗の和」の最小値
		vector<vector<ll>> dp(N+1, vector<ll>(OUTPUT_MAX+1, INF64));
		dp[0][128] = 0;  // DP初期値

		// 配るDP
		// dp[i][*] <-- dp[i-1][j] への遷移を行う
		//   diff:差の二乗、つまりj+code[]とinput_data[i]の差の二乗
		// として、code[]の各値について
		//   dp[i][j+code[*]] = min(dp[i-1][j] + diff);
		// となる。
		for(i = 1; i <= N; i++)  // i番目の入力信号
		{
			for(j = 0; j <= OUTPUT_MAX; j++)  // j:複合化後の出力信号
			{
				if(dp[i-1][j] == INF64) continue;  // 遷移元が遷移不可能であればスルー
				for(auto &e : code)  // e:コードブックの値
				{
					int num = j+e;
					// 0～255でクリップ
					num = max(num, 0);
					num = min(num, OUTPUT_MAX);

					int diff = (input_data[i] - num) * (input_data[i] - num);

					dp[i][num] = min(dp[i][num], dp[i-1][j] + diff);
				}
			}
		}
		
		// dp[N][*] の最小値が答
		ll answer = INF64;
		for(j = 0; j <= OUTPUT_MAX; j++)
		{
			answer = min(answer, dp[N][j]);
		}
		cout << answer << endl;
	}

	return 0;
}
