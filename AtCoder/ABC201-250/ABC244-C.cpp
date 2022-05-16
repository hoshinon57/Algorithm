#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC244 https://atcoder.jp/contests/abc244

int main(void)
{
	// 1-indexed
	int N;
	cin >> N;
	vector<bool> canUse(N*2+1+1, true);  // その数値をまだ使えるならtrue

	while(true)
	{
		for(int i = 1; i <= N*2+1; i++)
		{
			if(canUse[i])
			{
				cout << i << endl;
				canUse[i] = false;
				break;
			}
		}
		int aoki;
		cin >> aoki;
		if(aoki == 0) break;
		canUse[aoki] = false;
	}

	return 0;
}
