#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題72
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc034

/*
 * 横方向の移動マス数はW-1, 縦方向はH-1. よって移動回数は W-1+H-1=W+H-2 必要。
 * (W,H)のマスに行くには、移動回数のW+H-2のうち、横方向の移動タイミングをW-1個選ぶ選び方となる。
 * よって (W-1+H-1) C (W-1) を求める。
 * 
 * 以下のサイトを元にnCk_init(), nCk()を実装した。(内容は理解できていない)
 * 参考：
 *   https://drken1215.hatenablog.com/entry/2018/06/08/210000
 *   https://cod-aid.com/library-binomial-coefficient
 *   https://algo-logic.info/combination/
 */

const ll MOD = 1000000007;
const ll NCK_SIZE = 200001;  // N<=2*10^5 など、問題文の制約に合わせる
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
	ll W, H;
	cin >> W >> H;

	nCk_init();

	// 横方向の移動マス数はW-1, 縦方向はH-1.
	// (W-1+H-1) C (W-1) を求める。
	cout << nCk(W+H-2, W-1) << endl;

	return 0;
}
