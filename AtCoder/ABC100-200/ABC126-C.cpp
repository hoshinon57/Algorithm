#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC126 https://atcoder.jp/contests/abc126

/*
 * サイコロの出目で全探索する。
 * 出目をiとすると、これがK以上になるまで
 *   iを2倍
 *   勝つ確率を1/2倍
 * していく。
 * (各出目ごとに勝つ確率の初期値は1/N)
 * 
 * 上記の総和が答。
 */

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	double ans = 0.0;
	for(i = 1; i <= N; i++)
	{
		double d = 1.0/N;
		int j = i;
		while(j < K)
		{
			d /= 2;
			j *= 2;
		}
		ans += d;
	}
	cout << std::fixed << std::setprecision(10);
	cout << ans << endl;

	return 0;
}
