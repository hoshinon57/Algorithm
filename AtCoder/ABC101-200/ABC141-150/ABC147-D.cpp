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
 * bitごとに分けて考える。
 * ⇒「XORは他bitと干渉しないため、bitごとに独立に考えることができる」
 * 
 * まずA1とA2,A3,A4とのXOR&総和を考えると、例えばあるbitについて
 *   A1が0：A2,A3,A4にて1の数
 *   A1が1：A2,A3,A4にて0の数 (1とのXORをするため)
 * となる。
 * つまりbitごとに1および0の個数をカウントしておけば、計算できそう。
 * 
 * sum_01[i][j]を、下からibit目がj(0or1)である整数Aの個数 と定義する。
 * i=0から順に、AiとA(i+1)～A(N-1)とのXORを計算するにあたり、
 *   sum_01からAiのぶんを取り除く
 *   Aiの各bitについて、0ならばsum_01[i][1]を、1ならばsum_01[i][0]を加算
 * としていけばよい。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	// sum[i][j]:下からibit目がj(0or1)である整数Aの個数
	vector<vector<int>> sum_01(60, vector<int>(2, 0));  // sum_01[60][2]
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < 60; j++)
		{
			if( ((a[i]>>j)&1)==0 ) sum_01[j][0]++;
			else sum_01[j][1]++;
		}
	}

	ll ans = 0;
	for(i = 0; i < N; i++)  // AiとのXORを計算
	{
		// カウントからAiぶんを消す
		for(j = 0; j < 60; j++)
		{
			if( ((a[i]>>j)&1)==0 ) sum_01[j][0]--;
			else sum_01[j][1]--;
		}

		ll p = 1;
		for(j = 0; j < 60; j++)
		{
			// a[i]の該当bitが0ならば、相手は1のときにXORが1になる。
			// bitが1ならば、相手は0のときに同様。
			if( ((a[i]>>j)&1)==0 )
			{
				ans += sum_01[j][1] * p;
			}
			else
			{
				ans += sum_01[j][0] * p;
			}
			ans %= MOD;
			p = (p*2) % MOD;
		}
	}
	cout << ans << endl;

	return 0;
}
