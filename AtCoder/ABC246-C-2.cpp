#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ABC246 https://atcoder.jp/contests/abc246
// 尺取り法で解く版

long long func(long long a, long long b)
{
	return a*a*a+a*a*b+a*b*b+b*b*b;
}

int main(void)
{
	/*
	 * ・N<=10^18, a,bは非負の条件より、a,bともに10^6以下。
	 * ・aを0から大きくしていく方向に、
	 *   bを10^6から小さくしていく方向に動かしていく。
	 * [参考]
	 * https://atcoder.jp/contests/abc246/editorial/3701
	 */

	long long N;
	cin >> N;
	long long a, b;
	long long left, mid, right;
	long long answer = 1500000000000000000;  // 10^18より十分大きく
	long long tmp;

	b = 1000000;
	for(a = 0; a <= 1000000; a++)
	{
		while( b >= 0 )
		{
			tmp = func(a, b);
			if( tmp >= N )
			{
				answer = min(answer, tmp);
				b--;
			}
			else
			{
				break;  // X>=Nを満たさないため、aを加算
			}
		}
		if( b < 0 ) break;  // b=0まで計算したため終了
	}

	cout << answer << endl;

	return 0;
}
