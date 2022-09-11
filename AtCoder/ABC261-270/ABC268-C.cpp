#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC268 https://atcoder.jp/contests/abc268

/*
 * 初期状態にて、各人の料理が目の前からどれだけズレているかを計算し、その統計をcount[]に保持しておく。
 * 例)count[2]:料理のズレ量が2である人の数
 * 
 * テーブルをi回回したときに（正確には、1周の1/N回す操作をi回行ったときに）、
 *   ズレ量が(i-1), ズレ量がi, ズレ量が(i+1)
 * の3パターンの人が喜ぶ。
 * よってその3つの和を各iに対して計算し、その最大値が答となる。
 */
int main(void)
{
	// 0-indexed
	int i;
	int N;
	cin >> N;
	vector<int> p(N);

	// 各人の料理が目の前からどれだけズレているかを保持する。
	// 例)count[2]:料理のズレ量が2である人の数
	vector<int> count(N);
	for(i = 0; i < N; i++)
	{
		cin >> p[i];
		// 人iの料理が、目の前からどれだけズレているかを計算し、
		// その統計をcount[]に保持する。
		int x = ((i-p[i])+N)%N;
		count[x]++;
	}
	// ここまで入力

	// テーブルをi回回したときに、
	//   ズレ量が(i-1), ズレ量がi, ズレ量が(i+1)
	// の人は喜ぶ。
	// その3つの和を各iに対して計算し、その最大値が答となる。
	int answer = 0;
	for(i = 0; i < N; i++)
	{
		// iから3つの和を取る
		int sum = count[(i-1+N)%N] + count[i] + count[(i+1)%N];  // modに注意
		answer = max(answer, sum);
	}
	cout << answer << endl;

	return 0;
}
