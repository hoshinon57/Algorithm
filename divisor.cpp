#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 約数列挙

// Nの約数を列挙してdivisor[]に入れる
void func_divisor(long long N, vector<long long> &divisor)
{
	for(long long i = 1; i*i <= N; i++)
	{
		if( N%i == 0 )  // iで割り切れるなら約数
		{
			divisor.push_back(i);
			if( i*i != N )
			{
				// N=16, j=2のとき、8も約数になる。それを追加する。
				// (これを行うことで、約数列挙の計算量がO(N)からO(√N)になる)
				// ただしj*j=N(例：4*4=16)といった場合を
				// 追加してしまうと重複してしまうので、それは省く。
				divisor.push_back(N/i);
			}
		}
	}

	// ソート有無は必要に応じて
	sort(divisor.begin(), divisor.end());

	return;
}

int main(void)
{
	long long N;
	cin >> N;
	vector<long long> divisor;
	func_divisor(N, divisor);

	for(auto &&a : divisor)
	{
		cout << a << " ";
	}
	cout << endl;

	return 0;
}
