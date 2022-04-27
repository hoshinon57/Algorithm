#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ABC248 https://atcoder.jp/contests/abc248
// lower_bound(), upper_bound()を使わず、手動で二分探索してみる版

int main(void)
{
	/*
	 * ・事前に、A1～ANにて値xが何番目に登場するかをcount[x]に追加していく。(1始まり)
	 *   例：入力が"3 1 4 1 5"ならcount[1]={2,4}, count[2]={}, count[3]={1}
	 * ・各クエリに対して
	 *   count[X]でL以上が初めて出現するのは何番目か
	 *   count[X]でRより大きいものが初めて出現するのは何番目か
	 *   を計算し、その差分がXの個数となる。
	 * [参考]
	 * https://atcoder.jp/contests/abc248/editorial/3747
	 * https://blog.hamayanhamayan.com/entry/2022/04/16/230012
	 */

	int i;
	int N;
	cin >> N;
	vector<int> a(N);
	vector<vector<int>> count(200000+1, vector<int>());
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		count[a[i]].push_back(i+1);
	}

	int Q;
	cin >> Q;
	for(i = 0; i < Q; i++)
	{
		int L, R, X;
		cin >> L >> R >> X;
		int left, right, mid;
		// Lについて探索 L以上になる最初のものを探す
		left = -1;
		right = count[X].size();
		while(right-left > 1)
		{
			mid = (right+left) / 2;
			if(count[X][mid] < L)
			{
				left = mid;
			}
			else
			{
				right = mid;
			}
		}
		int leftIt = right;
		// Rについて探索 Rより大きくなる最初のものを探す
		left = -1;
		right = count[X].size();
		while(right-left > 1)
		{
			mid = (right+left) / 2;
			if(count[X][mid] <= R)
			{
				left = mid;
			}
			else
			{
				right = mid;
			}
		}
		int rightIt = right;
		cout << rightIt-leftIt << endl;
	}

	return 0;
}
