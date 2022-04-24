#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ABC249 https://atcoder.jp/contests/abc249

int main(void)
{
	int i, j, k;
	int N;
	cin >> N;
	vector<long> a(N);
	vector<long> count(200000+1);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		count[a[i]]++;
	}

	/*
	 * ・事前に、A1～ANに登場するxの数をcount[x]に保持しておく(count[2]なら2の登場回数)。
	 * ・iについて全探索する。
	 * ・各Aiの約数を列挙したときに(O√N)、約数がAjでAi/(約数)がAkとなる。
	 *   i,j,kの大小関係に制約は無いので、(Ajの出現回数)*(Ai/(約数)の出現回数)が、
	 *   その約数を使った時の組み合わせとなる。
	 * ・あとは約数ごと、Aiごとに加算していけば良い。
	 * [参考]
	 * https://blog.hamayanhamayan.com/entry/2022/04/23/225446
	 * https://qiita.com/chanhina/items/0713b53c8a10f6c0e065
	 * https://qiita.com/drken/items/a14e9af0ca2d857dad23
	 */

	int answer = 0;
	// iで全探索
	for(i = 0; i < N; i++)
	{
		vector<long> divisor;
		// a[i]の約数を列挙
		for(j = 1; j*j <= a[i]; j++)
		{
			if( a[i]%j == 0 )
			{
				divisor.push_back(j);
				if( j*j != a[i] )
				{
					// a[i]=16, j=2のとき、8も約数になる。それを追加する。
					// (これを行うことで、約数列挙の計算量がO(N)からO(√N)になる)
					// ただしj*j=a[i](例：4*4=16)といった場合を
					// 追加してしまうと重複してしまうので、それは省く。
					divisor.push_back(a[i]/j);
				}
			}
		}

		for(int j=0; j<divisor.size(); j++)
		{
			// Aj:divisor[j]
			// Ak:A[i]/Aj
			int b, c;
			b = count[divisor[j]];
			c = count[a[i]/divisor[j]];
			answer += b*c;
		}
	}

	cout << answer << endl;

	return 0;
}
