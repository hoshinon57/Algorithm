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
 * ABC171-F
 * ABC132-D
 * ABC042-D
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

// MODではなく正確な値を求めるような問題において、
// パスカルの法則を用いてnCkを計算するライブラリ
const int NCK_PASCAL_SIZE = 61;  // nCkのnの最大値 問題文の制約に合わせる
ll nCk_pascal_memo[NCK_PASCAL_SIZE][NCK_PASCAL_SIZE];
void nCk_Pascal_Init(void)
{
	int i, j;
	for(i = 0; i < NCK_PASCAL_SIZE; i++)
	{
		for(j = 0; j < NCK_PASCAL_SIZE; j++)
		{
			nCk_pascal_memo[i][j] = -1;
		}
	}
}
ll nCk_Pascal(int n, int k)
{
	// nCr = (n-1)Cr + (n-1)C(r-1)    ※パスカルの法則
	if(k == 0) return 1;
	if(n == k) return 1;
	if(nCk_pascal_memo[n][k] != -1) return nCk_pascal_memo[n][k];
	return nCk_pascal_memo[n][k] = nCk_Pascal(n-1, k) + nCk_Pascal(n-1, k-1);
}

int main(void)
{
	nCk_init();

	// Combinationの計算に使えるサイト：https://keisan.casio.jp/exec/system/1161228812
	// または、Google検索で "20 choose 10" でも計算できる
	cout << "10C2=" << nCk(10, 2) << endl;  // 45
	cout << "5C5=" << nCk(5, 5) << endl;  // 1
	cout << "1C1=" << nCk(1, 1) << endl;  // 1
	cout << "2C1=" << nCk(2, 1) << endl;  // 2
	cout << "100C10=" << nCk(100, 10) << endl;  // 752375420 (mod 998244353)

	/*
	 * 「MOD 998244353で計算する」ではないような問題について。
	 * 例：ABC202-D
	 * ＞(0,0)からスタートし、
	 * ＞x軸の正の方向あるいはy軸の正の方向に1進むことを繰り返して(i,j)に至る方法の総数に等しい
	 * から求めることができる。
	 * 60C30=10^17 近辺までなら計算できる。
	 * 実装参考：https://atcoder.jp/contests/abc202/submissions/47821692
	 */
	// MODを用いない範囲の問題について、パスカルの法則で解く方法。
	nCk_Pascal_Init();
	cout << "10C2=" << nCk_Pascal(10, 2) << endl;  // 45
	cout << "5C5=" << nCk_Pascal(5, 5) << endl;  // 1
	cout << "3C0=" << nCk_Pascal(3, 0) << endl;  // 1
	cout << "20C10=" << nCk_Pascal(20, 10) << endl;  // 184756
	cout << "60C30=" << nCk_Pascal(60, 30) << endl;  // 118264581564861424  この辺りが数値的に上限

	return 0;
}
