#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC147 https://atcoder.jp/contests/abc147

/*
 * 解説を元に実装した。
 *   https://blog.hamayanhamayan.com/entry/2019/12/13/214444
 *   https://img.atcoder.jp/abc147/editorial.pdf
 * 
 * bitごとにA1～ANのペアで 0 xor 1 となる組み合わせが何通りあるかを求める。
 * 0の個数をzero, 1の個数をoneとすると、上記は zero*one 通りとなる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = 0;
	ll p = 1;
	for(i = 0; i < 60; i++)
	{
		ll mask = 1LL << i;
		ll zero = 0, one = 0;  // bitが0,1の個数
		for(j = 0; j < N; j++)
		{
			if((a[j] & mask) == 0) zero++;
			else one++;
		}
		ans += (zero * one) % MOD * p;
		ans %= MOD;
		p = (p*2) % MOD;
	}
	cout << ans << endl;

	return 0;
}
