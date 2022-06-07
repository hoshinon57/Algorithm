#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC253 https://atcoder.jp/contests/abc253

/*
 * (1)1～Nの和
 * (2)1～Nのうち、Aの倍数の和
 * (3)1～Nのうち、Bの倍数の和
 * (4)1～Nのうち、(AとBの最小公倍数)の倍数の和
 * としたときに、
 * (1)-(2)-(3)+(4) が答え。
 */

ll gcd(ll a, ll b)
{
	if(a%b == 0)
	{
		return b;
	}
	else
	{
		return gcd(b, a%b);
	}
}

// 初項a, 末項end, 項数nの等差数列の和は n*(a+end)/2.
// 本問題の条件であれば、項数n=N/a, 末項end=n*aとなる。(nは小数点以下切り捨て)
// 参考：https://www.kwansei.ac.jp/hs/z90010/sugakua/suuretu/tousasum/tousasum.htm
ll sumsum(ll N, ll a)
{
	// 初項a
	ll n = N / a;  // 項数n
	ll end = n*a;  // 末項end
	return n*(a+end)/2;
}

int main(void)
{
	ll N, A, B, AB;
	cin >> N >> A >> B;
	AB = A * B / gcd(A, B);

	ll sumN = N*(N+1)/2;
	ll sumA = sumsum(N, A);
	ll sumB = sumsum(N, B);
	ll sumAB = sumsum(N, AB);

	cout << sumN - sumA - sumB + sumAB << endl;

	return 0;
}
