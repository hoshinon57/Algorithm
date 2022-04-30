#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ABC128 https://atcoder.jp/contests/abc128

int main(void)
{
	/*
	 * 以下、0-indexedで配列に値を入れていく。
	 * N個のスイッチそれぞれについてon/offの全パターンを調べる。2^N通り。
	 * 各パターンについて、電球に繋がっているスイッチがonの数をカウントし、
	 * p[]と比較して点灯するか調べる。
	 * 全て点灯するならカウント(answer)を増加。
	 */
	int i, j, k;
	int N, M;
	cin >> N >> M;
	vector<vector<int>> s(M, vector<int>(N, 0));  // s[M][N]  電球MがスイッチNに接続されているなら1
	vector<int> p(M);
	for(i = 0; i < M; i++)
	{
		cin >> k;
		for(j = 0; j < k; j++)
		{
			int tmp;
			cin >> tmp;
			s[i][tmp-1] = 1;  // 電球iがスイッチ(tmp-1)に接続されている
		}
	}
	for(i = 0; i < M; i++)
	{
		cin >> p[i];
	}

	int answer = 0;
	for(i = 0; i < (1<<N); i++)  // スイッチN個についてon/off全パターン探索
	{
		vector<int> count(M, 0);
		for(j = 0; j < N; j++)
		{
			if( ((i>>j)&1) == 0) continue;  // スイッチjはoffなのでcontinue
			for(k = 0; k < M; k++)
			{
				if(s[k][j] == 1) count[k]++;
			}
		}

		bool bOn = true;
		for(k = 0; k < M; k++)
		{
			if(count[k]%2 != p[k])
			{
				bOn = false;
				break;
			}
		}
		if(bOn)
		{
			// cout << "hoge:" << i << endl;
			answer++;
		}
	}

	cout << answer << endl;

	return 0;
}
