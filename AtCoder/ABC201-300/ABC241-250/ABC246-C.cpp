#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ABC246 https://atcoder.jp/contests/abc246

long long func(long long a, long long b)
{
	return a*a*a+a*a*b+a*b*b+b*b*b;
}

int main(void)
{
	/*
	 * ・N<=10^18, a,bは非負の条件より、a,bともに10^6以下まで探索すればよい。
	 * ・aを0～10^6まで全探索し、
	 *   各aについてbを二分探索する。
	 * [参考]
	 * https://blog.hamayanhamayan.com/entry/2022/04/03/012001
	 */

	long long N;
	cin >> N;
	long long a;
	long long left, mid, right;
	long long answer = 1500000000000000000;  // 10^18より十分大きく
	long long tmp;

	// aについて全探索
	for(a = 0; a <= 1000000; a++)
	{
		// bについて二分探索
		// leftはX>=Nの条件を常に満たさない、rightはX>=Nの条件を常に満たす
		left = -1;
		right = 1000000;
		while(right-left > 1)
		{
			mid = (left+right)/2;
			tmp = func(a, mid);
			if(tmp >= N)
			{
				right = mid;
				answer = min(answer, tmp);
			}
			else
			{
				left = mid;
			}
		}
	}

	cout << answer << endl;

	return 0;
}
