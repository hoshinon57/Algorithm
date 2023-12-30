#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC220 https://atcoder.jp/contests/abc220

/*
 * 各種解説を参考に、LCAの考え方で解く。
 *   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2021/0926_abc220
 *   https://atcoder.jp/contests/abc220/editorial/2929
 * 
 * 頂点の深さdを0-indexedとして書く。d:0～N-1.
 * 深さdの頂点vについて、これがLCAとなる組み合わせを計算する。
 * vは2^d個ある。
 * 
 * 大きく2パターンに分かれる。
 * (1)v自身がLCAになるケース
 * 相手はvのD個下の頂点となる。
 * d+D<=N-1が条件。葉より下に行ってしまうため。
 * 2^D個あり、頂点(i,j)と(j,i)は区別するため2倍、vは2^d個あるので、
 * 全体で2^D*2*2^d = 2^(D+d+1) となる。
 * 
 * (2)vの左右の子それぞれに頂点があるケース
 * lを新たに定義し、左側頂点の深さをd+lとする。
 *   左側：vのl個下 2^(l-1)個  ※片方の子しかカウントしないので-1
 *   右側：vの(D-l)個下 2^(D-l-1)個
 *   これらの組み合わせは 2^(D-2) 通り。 ★これを(X)とする。
 * ではlは何パターン取れるかを考える。
 * 左側をめいっぱい伸ばす：
 *   最大はd+l=N-1のときなので、l=N-d-1. これとD-1のminを取る。(右側にも1は下ろす必要があるので-1)
 *   これを(a)とする。
 * 右側をめいっぱい伸ばす：
 *   最大はd+D-l=N-1のときなので、l=d+D-N+1. これと1とのmaxを取る。(左側にも1は下ろす必要があるので-1)
 *   これを(b)とする。
 * lは(b)から(a)までの (a)-(b)+1 通り選べる。 ★これを(Y)とする。
 * ※ただし(a)<(b)のときは0通りとなる。(少なくとも左右の1つ、葉を超えてしまう)
 * (X)*(Y)に、(i,j)と(j,i)を区別するため2倍、また深さdの頂点は2^d個あるので、
 * 全体で (X)*(Y)*2*2^d となる。
 */

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	ll N, D; cin >> N >> D;
	ll d;
	ll ans = 0;
	for(d = 0; d <= N-1; d++)  // 深さdの頂点
	{
		ll sum = 0;
		// vがLCAになるケース
		if(d+D<=N-1)
		{
			sum += modpow(2, D+d+1, MOD);
		}

		// vの左右それぞれに頂点があるケース
		ll x = modpow(2, D-2, MOD);
		ll l1, l2;
		l1 = min(N-d-1, D-1);
		l2 = max(d+D-N+1, 1LL);
		if(l1 < l2) {;} // NG
		else
		{
			ll y = l1 - l2 + 1;
			ll z = x*y%MOD*modpow(2, d+1, MOD)%MOD;
			sum += z;
		}
		sum %= MOD;
		ans = (ans+sum) % MOD;
	}
	cout << ans << endl;

	return 0;
}
