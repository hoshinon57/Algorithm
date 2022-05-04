#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC085 https://atcoder.jp/contests/abc085/
// AtCoder Beginners Selectionより

int main(void)
{
	int N, Y;
	cin >> N >> Y;
	int a, b, c;  // それぞれ順に10000円札の枚数、5000円札の枚数、1000円札の枚数

	for(a = 0; a <= N; a++)
	{
		for(b = 0; b <= N; b++)
		{
			c = N - a - b;
			if( c < 0 ) break;

			if(Y == a*10000 + b*5000 + c*1000)
			{
				cout << a << " " << b << " " << c << endl;
				return 0;
			}
		}
	}

	cout << "-1 -1 -1" << endl;

	return 0;
}
