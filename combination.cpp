#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;

// Combination(組み合わせ、二項係数)

/*
 * 「n個のものから(順番を考慮せず)k個を選ぶ組み合わせ」であるnCkの計算。
 * 計算式は n!/(k!*(n-k)!) となる。
 * 競技プログラミングでは「998244353で割った余りを求めよ」になることが多い。
 * 
 * 単純な実装ではlong long型をオーバーしたり、MODにて除算を正しく計算できなかったりする。
 * そのため、
 * ・逆元を事前計算しておく
 * ・nCkの計算では除算ではなく逆元をかける
 * といった手法が必要になる。
 * 以下のサイトを元にnCk_init(), nCk()を実装した。
 * 参考：
 *   https://drken1215.hatenablog.com/entry/2018/06/08/210000
 *   https://cod-aid.com/library-binomial-coefficient
 *   https://algo-logic.info/combination/
 * 
 * 使い方：
 * ・問題文より、定数 MOD を設定する。
 * ・問題文の制約に合わせて NCK_SIZE を設定する。
 *   例えば nCk なら n! が出てくるので、fact[n]までのサイズが必要になる。
 * ・はじめに nCk_init() を呼び出しておく。
 * ・あとはnCk であれば nCk(n, k) を呼び出すことで、組み合わせの数をMODで割った余りを返す。
 * 
 * [関連する問題]
 * ABC132-D
 */

const ll MOD = 998244353;
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
	nCk_init();

	// Combinationの計算に使えるサイト：https://keisan.casio.jp/exec/system/1161228812
	cout << "10C2=" << nCk(10, 2) << endl;  // 45
	cout << "5C5=" << nCk(5, 5) << endl;  // 1
	cout << "1C1=" << nCk(1, 1) << endl;  // 1
	cout << "2C1=" << nCk(2, 1) << endl;  // 2
	cout << "100C10=" << nCk(100, 10) << endl;  // 752375420 (mod 998244353)

	return 0;
}
