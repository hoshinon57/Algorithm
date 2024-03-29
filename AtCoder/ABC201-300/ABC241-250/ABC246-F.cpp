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

// ABC246 https://atcoder.jp/contests/abc246

/*
 * 包除原理を用いて解く。公式解説とほぼ同じ実装だった。
 *   https://atcoder.jp/contests/abc246/editorial/3703
 *   https://drken1215.hatenablog.com/entry/2022/04/03/212800
 * 
 * 例えばN=2のとき、S1∪S2が答となる。
 * これは直接求めるのは難しいが、S1+S2-(S1∩S2)で求められる。
 * N=3以上のとき、
 *   奇数個の集合は答に加算
 *   偶数個の集合は答に減算
 * となる。
 * 
 * また、S1のみが与えられたとき、入力できる文字種は
 *   |S1|^L
 * となる。
 * 
 * よって解法としては、
 * Sの書く組み合わせについて(bit全探索を用いる)、Si∩Sj∩Sk... で作れる文字種を計算する。
 * 選んだSの数が奇数なら加算、偶数なら減算する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・制約のN=18より、bit全探索とか包除原理にアタリを付けられる。
 * ・例えばN=3にて、どういう値が答となるかを考えてみる。
 *   求めたいのはS1∪S2∪S3となるが、直接求めるのは難しそう。
 *   一方でS1∩S2∩S3は直接求めらえるので、包除原理が使えるのではないか。
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
	ll i, j;
	ll N, L; cin >> N >> L;
	vector<int> bi(N);  // bi[i]:a-zをbit0-25に割り当て、Siの使える文字をbitで表現する
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		for(auto &e : s)
		{
			bi[i] |= 1<<(e-'a');
		}
	}

	ll ans = 0;
	for(i = 1; i < 1<<N; i++)  // bit全探索
	{
		ll bmix = -1;  // all 1
		// 選ぶSで重複する文字を計算
		for(j = 0; j < N; j++)
		{
			if((i>>j)&1) bmix &= bi[j];
		}
		ll bcnt = __builtin_popcountll(bmix);
		ll n = modpow(bcnt, L, MOD);
		ll icnt = __builtin_popcountll(i);
		if(icnt%2 == 0) ans -= n;
		else ans += n;
		ans = (ans+MOD)%MOD;
	}
	cout << ans << endl;

	return 0;
}
