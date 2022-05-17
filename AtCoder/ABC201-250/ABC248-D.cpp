#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ABC248 https://atcoder.jp/contests/abc248

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
		auto it1 = lower_bound(count[X].begin(), count[X].end(), L);
		auto it2 = upper_bound(count[X].begin(), count[X].end(), R);
		auto answer = (it2-count[X].begin()) - (it1-count[X].begin());
		cout << answer << endl;
	}

	return 0;
}
