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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題74より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc021/tasks/abc021_d

/*
 * 自力で解けず、解説を見た。
 * 重複組み合わせの考え方で解く。
 *   https://manabitimes.jp/math/1101
 *   https://9sako6.com/posts/abc021_d
 * 
 * n=10, k=2のとき、
 *   9(=10-1)個の仕切りで1～10番の部屋に分けて、
 *   2個のボール(=a1,a1)を部屋に入れる
 * ことを考えると、仕切り9個とボール2個の並べ方の問題になり、 (11)C(2) となる。
 * これを一般化すると、 (n+k-1)C(k) が答となる。
 * よって
 *   (n+k-1)! / ( (k)!*(n-1)! )
 * を求めればよい。
 * 
 * 除算は逆元をかけて計算する。
 */

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

int main(void)
{
	ll i;
	ll n, k;
	cin >> n >> k;

	ll answer = 1;
	// (n+k-1) C (k) を求める
	for(i = 1; i <= n+k-1; i++)  // 分子の(n+k-1)!
	{
		answer *= i;
		answer %= MOD;
	}
	for(i = 1; i <= k; i++)  // 分母のk!
	{
		answer *= modinv(i, MOD);
		answer %= MOD;
	}
	for(i = 1; i <= n-1; i++)  // 分母の(n-1)!
	{
		answer *= modinv(i, MOD);
		answer %= MOD;
	}
	cout << answer << endl;

	return 0;
}
