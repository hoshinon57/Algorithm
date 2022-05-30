#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;
const ll MOD = 998244353;

// ABC238 https://atcoder.jp/contests/abc238

/*
 * 2桁のxについて、f(x) = x-9
 * 3桁のxについて、f(x) = x-99   となる。
 * 
 * f(1) + f(2) + ... + f(99)
 * = (1+2+...+9) + 10-9 + 11-9 + ... + 99-9
 * = (1+2+...+99) - (9*90)
 * f(1) + f(2) + ... + f(999)
 * = (1+2+...+9) + (10-9 + 11-9 + ... + 99-9) + (100-99 + 101-99 + ... + 999 - 99)
 * = (1+2+...+999) - (9*90) - (99*900)
 * となる。
 * 
 * 後はこれを適宜MODを取りながら計算する。
 */

int digit(ll N)
{
	if(N==0) return 0;
	int d = 0;
	while(N > 0)
	{
		d++;
		N /= 10;
	}
	return d;
}

int main(void)
{
	ll N;
	cin >> N;
	int N_d = digit(N);  // Nの桁数

	ll answer;
	if(N%2==0)
	{
		answer = (N/2%MOD)*((N+1)%MOD);
	}
	else
	{
		answer = (N%MOD)*((N+1)/2%MOD);
	}
	answer = answer % MOD;
//	answer = ((((N%MOD)*((N+1)%MOD))%MOD)*499122177)%MOD;  // 逆元をかけるこちらでも可

	// i=2, 3, ... のとき、
	// d=100, 1000, ...
	// sub1 = 9, 99, ...    f(N) = N - sub1となる値  ないし、1～(i-1)桁までの数の種類
	// sub2 = 99, 999, ...  1～i桁までの数の種類
	// sub2-sub1は、i桁の数の種類になる
	// sub1 * (min(N, sub2) - sub1)  を減算する   例：i=2なら 9 * (99-9) = 9 * 90
	ll d = 100;
	ll sub1, sub2;
	for(ll i = 2; i <= N_d; i++, d*=10)
	{
		sub1 = d / 10 - 1;
		sub2 = d - 1;
		ll tmp = (sub1%MOD) * ((min(N, sub2) - sub1)%MOD);
		tmp %= MOD;
		answer = answer + MOD - tmp;
		answer %= MOD;
	}

	cout << answer << endl;

	return 0;
}
