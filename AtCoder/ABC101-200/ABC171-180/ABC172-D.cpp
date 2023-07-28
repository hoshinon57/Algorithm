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

// ABC172 https://atcoder.jp/contests/abc172

/*
 * エラトステネスの篩のような方針で解く。
 * 
 * 問題文と同じく、f[i]:iの約数の個数 となる配列を用意する。
 * 各iについて、
 *   j=i～N以下の間で、iずつ増やしながらf[j]をインクリメント
 * とすれば全てのf[i]が求まる。
 */

int main(void)
{
	ll i, j;
	ll N; cin >> N;
	vector<ll> f(N+1);
	for(i = 1; i <= N; i++)
	{
		for(j = i; j <= N; j+=i)
		{
			f[j]++;
		}
	}
	ll ans = 0;
	for(i = 1; i <= N; i++)
	{
		ans += i*f[i];
	}
	cout << ans << endl;

	return 0;
}
