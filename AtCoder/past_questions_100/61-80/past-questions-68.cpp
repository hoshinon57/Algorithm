#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int MOD = 10000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題68
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_1_A&lang=ja

/*
 * 入力Nに対して、2から√Nの各整数で割れるだけ割っていく。
 * 割った数が素因数となる。
 * 
 * また、割り切った後の値が1以外であれば、それも素因数となる。
 * ※この値は必ず素数。合成数であれば、それを構成する素因数には√N以下の値が含まれており、
 *   これはここまでの「各整数で割れるだけ割っていく」に矛盾するため。
 * 
 * 参考：
 * https://qiita.com/drken/items/a14e9af0ca2d857dad23
 * https://algo-method.com/descriptions/119
 */

int main(void)
{
	int n, n_org;
	cin >> n;
	n_org = n;

	cout << n_org << ":";
	for(int i = 2; i*i <= n_org; i++)  // 2から√Nまで
	{
		while(n%i == 0)  // 割れる限り割る
		{
			n /= i;
			cout << " " << i;
		}
	}
	if(n != 1)  // 最後に残った値が1以外であれば、それも素因数
	{
		cout << " " << n;
	}
	cout << endl;

	return 0;
}
