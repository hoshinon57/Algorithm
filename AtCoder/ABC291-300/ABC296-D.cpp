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

// ABC296 https://atcoder.jp/contests/abc296

/*
 * まず、a<=bとして良い。
 * するとaについて全探索を考えたときに、a=10^6まで探索すれば
 * M<=abとなる値は漏れなく見つかると考える。
 * 
 * b>=M/aより、bはM/aの切り上げが最小となる。
 * aについての全探索について、a*bの最小値を保持していけばよい。
 */

int main(void)
{
	ll N, M; cin >> N >> M;
	ll ans = INF64;
	ll a, b;
	for(a = 1; a <= 1000000; a++)
	{
		b = (M+a-1) / a;
		if(a > N || b > N) continue;
		ans = min(ans, a*b);
	}
	if(ans == INF64) ans = -1;
	cout << ans << endl;

	return 0;
}
