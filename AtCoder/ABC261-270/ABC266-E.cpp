#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC266 https://atcoder.jp/contests/abc266

/*
 * まず、ダイスを1回振ったときの出目の期待値は、(1+2+3+4+5+6)/6=3.5 となる。
 * 
 * N=2を考える。1ターン目で、
 *   出目が1,2,3：ゲームを続行して2ターン目も振るのが最適（残り1ターン振ったときの期待値が3.5なので）。
 *   出目が4,5,6：ゲームを終了するのが最適。
 * よってスコアの期待値は、(3.5+3.5+3.5+4+5+6)/6=4.25 となる。
 * 
 * N=2の結果を元に、N=3を考える。1ターン目で
 *   出目が1,2,3,4：ゲームを続行して2ターン目も振るのが最適（残り2ターン振ったときの期待値が4.25なので）。
 *   出目が5,6：ゲームを終了するのが最適。
 * よってスコアの期待値は、(4.25+4.25+4.25+4.25+5+6)/6=4.6666... となる。
 * 
 * 以上のように、小さい方のNから順に計算していけばよい。
 * 入力Nに対して、N-1までの期待値をexp_val[N-1]とすると、
 *   出目がexp_val[N-1]よりも小さければゲーム続行、
 *   大きければゲーム終了
 * として計算していく。
 */
int main(void)
{
	int i, j;
	int N;
	cin >> N;
 
	vector<double> exp_val(N+1);  // exp_val[i]:最大iターンのときの期待値。i=1なら 1+2+3+4+5+6)/6=3.5 となる。
	// score[i][j]:iターン目の出目がj
	// i:1～N
	// j:1～6
	vector<vector<double>> score(N+1, vector<double>(6+1));
	for(i = 1; i <= N; i++)
	{
		double tmp = 0;
		for(j = 1; j <= 6; j++) // 出目
		{
			if(j < exp_val[i-1])  // 今回の出目よりも、以降の出目の期待値の方が大きい場合
			{
				// ゲームを続行して次ターンも振るのが最適
				// 以降のターンの期待値を記録する
				score[i][j] = exp_val[i-1];
			}
			else
			{
				// ゲームを終了するのが最適
				// 今回の出目を記録する
				score[i][j] = j;
			}
			tmp += score[i][j];
		}
		exp_val[i] = tmp/6;
	}
	cout << std::fixed << std::setprecision(10);
	cout << exp_val[N] << endl;

	return 0;
}
