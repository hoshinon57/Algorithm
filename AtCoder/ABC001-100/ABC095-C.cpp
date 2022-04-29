#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ABC095 https://atcoder.jp/contests/abc095/

int main(void)
{
	/*
	 * ABピザを「一切使わない」から「AピザBピザに全て使う」まで全探索する。
	 * ABピザをi枚買うとき、
	 *   Aピザ：X-i/2 枚
	 *   Bピザ：Y-i/2 枚
	 * となる。（ただし最小0でクリップ）
	 */
	long long A, B, C, X, Y;
	cin >> A >> B >> C >> X >> Y;
	long long i;
	
	long long answer = 10000000000;  // 十分大きい数
	for(i = 0; i <= max(X, Y)*2; i+=2)  // ABピザの枚数を、「一切使わない」から「AピザBピザに全て使う」までループ
	{
		long long num_a, num_b;
		num_a = max(X-i/2, 0LL);
		num_b = max(Y-i/2, 0LL);
		auto tmp = num_a * A + num_b * B + i * C;
		answer = min(answer, tmp);
	}

	cout << answer << endl;
	
	return 0;
}
