#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC346 https://atcoder.jp/contests/abc346

/*
 * コンテスト中にACはしたが、非常に苦戦した。
 * こういう「前後から組み合わせる」系が苦手だと反省。
 * 
 * 完成した良い文字列は、
 *   010100101
 *   010110101
 *   101011010
 *   101001010
 * のようにある1か所だけ同じ文字が連続し、その前後は0101..が続く形となる。
 * 
 * よって結論としては、
 *   sum_0_fr[i]:010101... の先頭からi文字を作るときのコスト
 *   sum_1_rv[i]:101010... の末尾からi文字を作るときのコスト
 * とし、これは累積和の要領で計算する。
 * これを使い、先頭からi文字、後半からN-i文字を組み合わせるときのコストは
 *   sum_0_fr[i]+sum_1_rv[N-i]
 * で計算できるので、これを各i(1～N-1)について計算すればよい。
 * 
 * 上記だけでは先頭が0のケースしか求められないが、
 *   costA = 010110 にするためのコスト
 *   costB = 101001 に同様
 * とすると、costB = ΣC - costAで簡単に求められる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・出来上がりの形(良い文字列)をリストアップすると、パターンは少なそうというのが分かる。
 * ・前後からの累積和を計算しておいて、ある場所を境界に前側と後側を組み合わせるという考え方。
 */

int main(void)
{
	// 0-indexed
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	vector<ll> c(N); for(i = 0; i < N; i++) {cin >> c[i];}
	ll c_sum = reduce(c.begin(), c.end());

	vector<ll> sum_0_fr(N+1), sum_1_rv(N+1);  // [i]:先頭i文字を 01010... にするときのコスト, 末尾からi文字を 10101... に同様
	for(i = 0; i < N; i++)
	{
		sum_0_fr[i+1] = sum_0_fr[i];
		// 01010...
		if(i%2 != s[i]-'0')  // i文字目のあるべき値は(i%2)
		{
			sum_0_fr[i+1] += c[i];
		}

		sum_1_rv[i+1] = sum_1_rv[i];
		// 10101...
		if((N-i-1+1)%2 != s[N-i-1]-'0')  // 末尾からi文字目のあるべき値は(N-i-1 +1)%2 最後の+1はiが偶数番目のとき1にするため
		{
			sum_1_rv[i+1] += c[N-i-1];
		}
	}

	// 先頭からi文字、末尾からN-i文字を組み合わせる (i=1～N-1)
	ll ans = INF64;
	for(i = 1; i <= N-1; i++)
	{
		ll tmp = sum_0_fr[i] + sum_1_rv[N-i];
		// 例えば 11010 を作りたいとき、(ΣC-tmp)が 00101 のコストになる
		chmin(ans, min(tmp, c_sum-tmp));
	}
	cout << ans << endl;

	return 0;
}
