#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC226 https://atcoder.jp/contests/abc226

// 各i,jの組についてdx,dyを求める
// dx,dyをgcdで割ってset<pair<ll,ll>>へ
// setのサイズが答
// 
// gcdにてabsを用いたが、不要だったっぽい

// a,bの最大公約数を返す
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> x(N), y(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
	}

	set<pair<ll,ll>> mag;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)  // 町i->j
		{
			if(i == j) continue;
			ll dx = x[j] - x[i];
			ll dy = y[j] - y[i];
			ll g = gcd(abs(dx), abs(dy));
			dx /= g;
			dy /= g;
			mag.insert({dx, dy});
		}
	}
	cout << mag.size() << endl;
	return 0;
}
