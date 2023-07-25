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

// ABC230 https://atcoder.jp/contests/abc230

/*
 * ⌊N/i⌋が同じ値になるようなiの範囲を考える。
 * 
 * j=N/i, k=N/jとすると、
 * i=[i,k]について ⌊N/i⌋ の値が同じとなる。
 * よって[i,k]についてまとめて和を計算して、次はi+1から計算していけばよい。
 * 
 * 計算量はO(√N)になる…らしい。
 * √N < i <= N
 * ⇒ 1/√N < i/N <= 1
 * ⇒ 1 <= N/i < √N
 * より、⌊N/i⌋として取りうる値の種類は√N個という考え方。
 *   参考:https://kanpurin.hatenablog.com/entry/2021/12/07/211249
 */
int main(void)
{
	ll N;
	cin >> N;
	ll ans = 0;
	ll i;
	for(i = 1; i <= N; i++)
	{
		ll j = N/i;
		ll k = N/j;
		// x=[i,k]が N/x の同じ値
		ans += (N/i) * (k-i+1);
		i = k;
	}
	cout << ans << endl;

	return 0;
}
