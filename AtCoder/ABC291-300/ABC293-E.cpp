#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC293 https://atcoder.jp/contests/abc293

/*
 * ダブリングで解く。
 * 
 * Xのいくつかを実際に書いてみると、以下となる。
 *   X=1:A^0=1
 *   X=2:A^0+A^1
 *       =(A^0)*(1+A^1)
 *       =(X==1)*(1+A^1)
 *   X=3:A^0+A^1+A^2
 *       =(X==2)*A+1
 *   X=4:A^0+A^1+A^2+A^3
 *       =(A^0+A^1)*(1+A^2)
 *       =(X==2)*(1+A^2)
 *   X=5:(X==4)*A+1
 *   X=6:(X==3)*(1+A^3)
 * X->2Xのように2倍になると、1+A^Xをかけたものになり、
 * X->X+1のように+1になると、Aをかけて+1したものになる。
 * 
 * 例えばX=13 (b'1101) であれば、
 *   1->2->3->6->12->13
 * のように上の桁から1ずつ見ていって、
 * ・まず2倍
 * ・値が1なら+1
 * という操作を繰り返していけばよい。
 */

int main(void)
{
	ll A, X, M;
	cin >> A >> X >> M;

	// 1101(2)であれば 1->1->0->1 のように上の桁から見ていきたいので、
	// x_bit[]にまず下の桁から入れていき、最後に反転
	vector<int> x_bit;
	while(X > 0)
	{
		x_bit.push_back(X&1);
		X >>= 1;
	}
	std::reverse(x_bit.begin(), x_bit.end());

	ll a_pow = 0;
	ll answer = 0;
	for(int i = 0; i < (int)x_bit.size(); i++)
	{
		// 初回
		if(i == 0)
		{
			answer = 1;
			answer %= M;  // X=1かつM=1のとき答は0
			a_pow = A;
			continue;
		}

		// i=1以降
		// まず2倍操作
		answer *= 1+a_pow;
		answer %= M;
		a_pow *= a_pow;
		a_pow %= M;
		// +1操作
		if(x_bit[i] == 1)
		{
			answer = answer*A + 1;
			answer %= M;
			a_pow *= A;
			a_pow %= M;
		}
	}
	cout << answer%M <<endl;

	return 0;
}
