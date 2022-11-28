#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 100000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題90
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/s8pc-5/tasks/s8pc_5_b

/*
 * 半径が決まっていない円について全探索する。
 * 
 * ・半径が決まっている円A
 * ・半径が決まっていない円B
 * と区別すると、以下が言える。
 * A,Bの比較：
 *   A,Bの中心どうしの距離からAの半径を引いたものが、Bの半径の最大値
 * B,B'の比較：
 *   B,B'の中心どうしの距離を二等分したものが、Bの半径の最大値
 * 
 * よってBの円それぞれについて、他の全ての円との比較をして、条件を満たす半径の最大値を求める。
 * これをBの円全てについて求め、その最小値が答となる。
 * 
 * テストケース3のようにBが0個のケースもあるため、
 * 答の初期値はAの円のうち半径が最小のものにしておくと良い。
 * 
 * 計算量はO(M(N+M)).
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int N, M;
	cin >> N >> M;
	double answer = (double)INF32;
	vector<double> x(N+M), y(N+M), r(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i] >> r[i];
		answer = min(answer, r[i]);  // 答の初期値は、半径が決まっている円のうち最小の半径にしておく
	}
	for(i = N; i < N+M; i++) cin >> x[i] >> y[i];

	// 円aと円bの中心の距離を返す
	auto calc = [&](int a, int b) -> double
	{
		double x_tmp = x[a]-x[b];
		double y_tmp = y[a]-y[b];
		return sqrt(x_tmp*x_tmp + y_tmp*y_tmp);
	};

	// i:半径が決まっていない円N～M-1
	for(i = N; i < N+M; i++)
	{
		double ci_answer = (double)INF32;  // 円iについて、条件を満たすような半径の最大値
		double dist;

		// j:半径が決まっている円0～N-1
		for(j = 0; j < N; j++)
		{
			// 円jは半径が決まっているので、
			// 中心どうしの距離から半径を引いたものが、円iの半径の最大値
			dist = calc(i, j);
			ci_answer = min(ci_answer, dist - r[j]);
		}

		// j:半径が決まっていない円i+1～N+M-1
		for(j = i+1; j < N+M; j++)
		{
			// 円i,jともに半径が決まっていないので、
			// 中心どうしの距離を二等分したものが、円iの半径の最大値
			dist = calc(i, j);
			ci_answer = min(ci_answer, dist/2);
		}

		answer = min(answer, ci_answer);
	}

	cout << std::fixed << std::setprecision(10);
	cout << answer <<endl;

	return 0;
}
