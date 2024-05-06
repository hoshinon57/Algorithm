#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ABC057 https://atcoder.jp/contests/abc057/

int digit(long long n)
{
	int count = 0;
	while(n > 0)
	{
		count++;
		n /= 10;
	}

	return count;
}

int func(long long a, long long b)
{
	return max(digit(a), digit(b));
}

int main(void)
{
	/*
	 * まず、A<=Bの範囲に絞っても問題無い。
	 * AはNの約数になるので、√N以下の約数を列挙する -> divisor[]
	 * 次に各約数について、約数(=A)とN/約数(=B)の桁数のうち大きい方を取得し、
	 * その最小値を求めればよい。
	 */
	long long N;
	cin >> N;
	vector<int> divisor;
	long long i;

	for(i = 1; i*i <= N; i++)  // N=16としたときに、i=8,16は追加不要
	{
		if(N%i == 0)
		{
			divisor.push_back(i);
		}
	}

	int answer = 100; // 桁数として十分大きい数
	for(i = 0; i < divisor.size(); i++)
	{
		// cout << divisor[i] << endl;
		int tmp;
		tmp = func(divisor[i], N/divisor[i]);
		answer = min(answer, tmp);
	}

/*
	cout << digit(10) << endl;
	cout << digit(99) << endl;
	cout << digit(100) << endl;
	cout << digit(9999) << endl;

	cout << func(1, 1) << endl;
	cout << func(1, 1000) << endl;
	cout << func(1000, 1) << endl;
	cout << func(10000, 10000) << endl;
*/
	cout << answer << endl;
	return 0;
}
