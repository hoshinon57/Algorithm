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
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC220 https://atcoder.jp/contests/abc220

/*
 * 根からの深さごとに数え上げして解く。
 * 以下、頂点の深さを1-indexedで記載する。(=根が深さ1)
 * 
 * ある頂点vに着目し、その深さをkとすると、距離がDである頂点は以下の3パターンに区別できる。
 * (1)vからD個下にある頂点
 * (2)vからD個上にある頂点
 * (3)vからx個上に行き、さらにそこから逆方向へ(D-x)個下にある頂点
 * 
 * (1)はk+D<=Nという条件で2^D個ある。
 * (2)はk-D>=1という条件で1個。
 * (3)は複雑で、
 * xの最小：
 *   逆方向へ(D-x)個下に行った先がN以下である必要があるので、
 *   k-x+(D-x)<=N   を整理すると (k+D-N)/2 の切り上げ、
 *   または1との最大となる。
 * xの最大：
 *   x個上に行くときに根までしか行けないので、x<=k-1.
 *   (2)よりD-1までにしか上に行けないので、x<=D-1.
 *   この最小となる。
 * 実装では、関数f()にて前者をa, 後者をbとしている。
 * すると(3)の頂点数は初項を2^(D-b-1), 比を2, 項数を(b-a+1)とする等比数列となるので、
 * (3)の頂点数は結局 (2^(b-a+1) -1)*c となる。
 * 
 * 深さkの頂点の1つであるvに着目したとき、(1)+(2)+(3)が距離Dである頂点数となる。
 * 深さkの頂点数は 2^(k-1) 個あるので、
 * ( (1)+(2)+(3) ) * 2^(k-1) 個となる。
 * これを深さkが1～Nまでそれぞれ計算したときの総和が答。
 * 
 * [ACまでの思考の流れ]
 * ・制約と木の構造から、各深さごとに計算して総和を求める感じっぽい。
 * ・あとは手を動かして、各深さごとにどれだけペアがあるかを立式していった。大変だった。
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

// 深さkの頂点から、x個上がって(D-x)個下にある頂点数の、全xについての総和を返す
ll f(ll N, ll D, ll k)
{
	if(D == 1) return 0;

	ll a = max((k+D-N+1)/2, 1LL);  // (k+D-N)/2の切り上げ
	ll b = min(k-1, D-1);
	ll c = modpow(2, D-b-1, MOD);
	ll ans = (modpow(2, b-a+1, MOD) + MOD - 1) * c;  // 等比数列の総和
	ans %= MOD;
	return ans;
}

int main(void)
{
	int k;
	ll N, D; cin >> N >> D;
	ll ans = 0;
	for(k = 1; k <= N; k++)  // 深さkとなる頂点
	{
		ll a1=0, a2=0, a3=0;
		// D個下
		if(k+D <= N)
		{
			a1 = modpow(2, D, MOD);
		}
		// D個上
		if(k-D >= 1)
		{
			a2 = 1;
		}
		// x個上がって(D-x)個下の総和
		a3 = f(N, D, k);

		ll n = modpow(2, k-1, MOD);  // 深さkの頂点数

		ans += (a1+a2+a3)*n;
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
