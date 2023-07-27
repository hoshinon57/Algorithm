#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC260 https://atcoder.jp/contests/abc260

/*
 * シミュレーションで解く。
 * 
 * レベルの高い方から順に、
 * ・レベル(n)の赤い宝石を、レベル(n-1)の赤い宝石1個とレベル(n)の青い宝石X個に変換
 * ・レベル(n)の青い宝石を、レベル(n-1)の赤い宝石1個とレベル(n-1)の青い宝石Y個に変換
 * を行っていく。
 * 
 * 赤い宝石の変換から先に必要。
 * 赤い宝石は、同レベルの青い宝石に変換できるため。
 */

int main(void)
{
	// 1-indexed
	int i;
	int N, X, Y;
	cin >> N >> X >> Y;

	vector<ll> red(N+1), blue(N+1);
	red[N] = 1;  // 初期所持
	for(i = N; i >= 2; i--)
	{
		// レベル(n)の赤い宝石を変換
		red[i-1] += red[i];
		blue[i] += red[i] * X;
		red[i] = 0;  // 変換後は0個になる(無くても良い)

		// レベル(n)の青い宝石を変換
		red[i-1] += blue[i];
		blue[i-1] += blue[i] * Y;
		blue[i] = 0;  // 変換後は0個になる(無くても良い)
	}
	cout << blue[1] << endl;

	return 0;
}
