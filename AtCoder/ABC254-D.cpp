#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC254 https://atcoder.jp/contests/abc254

/*
 * まずこの回答はTLEとなる。
 * 問題を見て最初に思いついた手順。
 * 
 * 1～(N*N)までの各平方数について調査する。
 * 平方数をaとしたときに、aの約数を列挙する。約数をiとするとj=a/i.
 * このi,jがN以下であればカウントする。
 * 
 * その際、(i,j)と(j,i)は別カウントになるため、
 * (1)1以上 √a未満 について約数列挙
 * (2)√a
 * (3)√aより大きい値 について約数列挙
 * についてそれぞれ約数列挙をすると、N以下という条件を満たす数は(1)(3)で同じとなる。
 * そのため(1),(2)のみ計算すればよい。
 * 
 * このアルゴリズムだと計算量がO(N^2)となってしまい、TLEになる。
 */
int main(void)
{
	ll answer = 0;
	ll k, i, j;
	ll N;
	cin >> N;
	for(k = 1; k <= N; k++)
	{
		ll tmpCount = 0;
		// 1～N*Nまでの各平方数について調査
		ll a = k*k;
		for(i = 1; i*i < a; i++)  // aの約数列挙 √aは別途計算するので i*i<a まで
		{
			if(a%i == 0)
			{
				j = a/i;
				if(i <= N && j <= N) tmpCount++;  // N以下という条件
			}
		}
		// i*i(=j)=aの調査
		if(i <= N) answer++;

		// √aより大きい値についてはi,jが逆になるパターンなので、
		// 条件を満たす数はtmpCountと同じ よって2倍した値を加算する
		answer += tmpCount*2;
	}

	cout << answer << endl;

	return 0;
}
