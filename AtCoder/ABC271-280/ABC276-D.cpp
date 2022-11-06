#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC276 https://atcoder.jp/contests/abc276

/*
 * 数列A全体の最大公約数をまず求める。以下これをdivとする。
 * 
 * 目標を達成するには、全てのa[i]について
 *   a[i] = div * 2^X * 2^Y  (X,Y>=0)
 * と表せることが条件。
 * よって手順としては、各a[i]に対して
 *   (1)まずdivで割る
 *   (2)2および3で割れるだけ割る
 *   (3)a[i] != 1 なら目標を達成できない(NG)
 * となる。
 * 目標を達成できる場合、(2)で割った回数が答となる。
 */

ll gcd(ll a, ll b)
{
	if(b == 0) return a;
	return gcd(b, a%b);
}

int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<int> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}

	ll div;  // 数列A全体の最大公約数
	div = a[0];
	for(i = 0; i < N-1; i++)
	{
		div = gcd(div, a[i+1]);
	}

	ll answer = 0;
	for(i = 0; i < N; i++)
	{
		// 各a[i]について、
		//   まず全体の最大公約数で割る
		//   2および3で割れるだけ割る
		//   a[i] != 1 なら目標を達成できない(NG)
		a[i] /= div;
		while(a[i]%2 == 0)
		{
			a[i] /= 2;
			answer++;
		}
		while(a[i]%3 == 0)
		{
			a[i] /= 3;
			answer++;
		}
		if(a[i] != 1)
		{
			cout << -1 << endl;
			return 0;
		}
	}
	cout << answer << endl;

	return 0;
}
