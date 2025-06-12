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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC405 https://atcoder.jp/contests/abc405

/*
 * コンテスト時に解けず、解説を見た。
 *   https://x.com/kyopro_friends/status/1921200308975288371
 *   https://atcoder.jp/contests/abc405/editorial/13004
 * 
 * (1)
 * kyopro_friends氏の方針とほぼ同じ。
 * まずaとdを並べる。これはaaa...aaaddd...ddd と一意に決まるのがポイント。
 * 次にcをk個、aとdの間に差し込む。
 * すると(C-k)個の残りのcは、dの2個目以降と混ぜることになる。
 * ⇒(D-1)個のdと(C-k)個のcを並べるので、C(D-1+C-k,D-1)となる。
 * 
 * bはdより前なので、A個のaとk個のcと混ぜることになる。
 * ⇒(A+k)個のa,cとB個のbを並べるので、C(A+k+B,B)となる。
 * 
 * 上記2つのCombinationの積を、k:0～Cごと計算した和が答。
 * 
 * (2)
 * 他にも手順あるかな～と考えた方法。
 * まずaとdを並べる。
 * aが終わるまでにbがk個使われている、とする。
 * (A-1個のaとk個のb) a (B-k個のb) ddd...ddd となる。
 * a,b部分の並べ方は C(A-1+k,k)となる。
 * 
 * cはaより後なので、先ほどの (B-k個のb) ddd...ddd に混ぜる。
 * C(B-k+D+C,C)となる。
 * 
 * 同じように積をとり、k:0～Bごと計算した和が答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・これむっずいな… 何らか全探索するのはさすがに見えたが、そこからが全然だった。
 * ・互いに干渉しないいくつかの要素をまず並べて、
 *   残りについて何らか全探索を考える、という方針。
**/

const ll MOD = 998244353;
//const ll NCK_SIZE = 200001;  // N<=2*10^5 など、問題文の制約に合わせる
const ll NCK_SIZE = 4e6+10;
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

int main(void)
{
	nCk_init();
	ll A, B, C, D; cin >> A >> B >> C >> D;
	ll k;
	mint ans = 0;
#if 0  // kyopro_friends氏に近い解法
	for(k = 0; k <= C; k++)
	{
		mint tmp1 = nCk(D-1+C-k, D-1);
		mint tmp2 = nCk(A+k+B, B);
		ans += tmp1*tmp2;
	}
#else  // 後に自分で考えた解法
	for(k = 0; k <= B; k++)
	{
		mint tmp1 = nCk(A-1+k, k);
		mint tmp2 = nCk(B-k+D+C, C);
		ans += tmp1*tmp2;
	}
#endif
	cout << ans.val() << endl;

	return 0;
}
