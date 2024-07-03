#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC232 https://atcoder.jp/contests/abc232

/*
 * 順列を全探索すれば解ける問題にて、計算量を削減したいときにbitDPが使える。 [典型]
 * 
 * 考え方としては、
 *   まずswapしてA,Bの位置を揃えてから
 *   ±1操作にてA,Bの値を合わせる
 * となる。
 * 
 * dp[S]を
 *   iビット目をa[i]に対応させ、ビットが1:a[i]を使う、0:a[i]を使わない とし、
 *   pc個使うとしてb[0]～b[pc-1]までに対応させるときの、
 *   コスト最小値
 * と定義する。
 * dp[(1<<N)-1]が答。
 * 
 * これをメモ化再帰で実装する。
 *   pc:Sにてビットが立っている個数
 *   Sprev:Sからビットを1つ落としたもの  ※iビット目とする
 * とすると、iを(pc-1)目のビットに移動させることになる。  ※これをjとする
 * ただしi以外のswap操作にて、iより大きいbitの個数ぶん、iは上位bitに移動している。
 * これをcntとすると、 i-j+cnt だけ移動させることになる。
 * 
 * よって各iについて、 dp[Sprev] + Y*(i-j+cnt) + X*|b[j]-a[i]| の最小値がdp[S]となる。
 */

ll N, X, Y;
vector<ll> a, b;
vector<ll> dp;

ll bitdp(int S)
{
	ll &d = dp[S];
	if(d != -1) return d;

	int pc = __builtin_popcount(S);
	if(pc == 0) return (d=0);  // ベースケース

	d = INF64;
	for(int i = 0; i < N; i++)
	{
		if(((S>>i)&1) == 0) continue;
		int Sprev = S & (~(1<<i));  // iビット目を落とす
		int j = pc-1;  // jビット目に移動させる

		// iビット目より上にて立っているビットを数える
		// この数だけ、既上位bitへ移動している
		int cnt = __builtin_popcount(Sprev & (~ ((1<<i)-1)) );

		ll tmp1 = Y * (i-j+cnt);
		ll tmp2 = X * abs(b[j]-a[i]);
		ll tmp3 = bitdp(Sprev);
		chmin(d, tmp1+tmp2+tmp3);
	}
	return d;
}

int main(void)
{
	int i;
	cin >> N >> X >> Y;
	a.resize(N);
	b.resize(N);
	for(i = 0; i < N; i++) cin >> a[i];
	for(i = 0; i < N; i++) cin >> b[i];
	dp.resize(1<<N, -1);

	cout << bitdp((1<<N)-1) << endl;

	return 0;
}
