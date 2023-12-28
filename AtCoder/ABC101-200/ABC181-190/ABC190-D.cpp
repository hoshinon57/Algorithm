#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC190 https://atcoder.jp/contests/abc190

/*
 * 数学問題。shakayami氏の解説と近かった。
 *   https://atcoder.jp/contests/abc190/editorial/643
 * 
 * 初項a, 項数b, (公差1) の等差数列の和は (a+(a+b-1))*b/2 となり、これがNとなる。
 * bは整数なので、N*2の約数がb候補となる。
 * b候補となる約数をeとすると、
 *   a+(a+b-1) = N*2/e (=Xとおく)
 *   a = (X-b+1)/2
 * となるaが整数であればよい。
 */

// Nの約数を列挙してdivisor[]に入れる
// 参考：https://qiita.com/drken/items/a14e9af0ca2d857dad23
void func_divisor(long long N, vector<long long> &divisor)
{
	for(long long i = 1; i*i <= N; i++)
	{
		if( N%i == 0 )  // iで割り切れるなら約数
		{
			divisor.push_back(i);
			if( i*i != N )
			{
				// N=16, j=2のとき、8も約数になる。それを追加する。
				// (これを行うことで、約数列挙の計算量がO(N)からO(√N)になる)
				// ただしj*j=N(例：4*4=16)といった場合を
				// 追加してしまうと重複してしまうので、それは省く。
				divisor.push_back(N/i);
			}
		}
	}

	// ソート有無は必要に応じて
	sort(divisor.begin(), divisor.end());

	return;
}

int main(void)
{
	ll N; cin >> N;
	vector<ll> divisor;
	func_divisor(N*2, divisor);
	int ans = 0;
	for(auto &e : divisor)
	{
		ll x = N*2/e;
		x = x-e+1;
		if(x%2==0) ans++;
	}
	cout << ans << endl;

	return 0;
}
