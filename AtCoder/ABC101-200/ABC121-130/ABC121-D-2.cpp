#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC121 https://atcoder.jp/contests/abc121

/*
 * O(1)実装。公式解説やけんちょん氏の記事など。
 *   https://drken1215.hatenablog.com/entry/2019/03/09/224100
 * 
 * nが偶数のとき、n^(n+1)=1.
 * よって先頭を0として、4つ取るとxorは1^1=0になる。つまり4つでループ。
 * 残りは高々3つなので、計算してしまう。先頭は(n/4)*4で計算できる。
 */

ll g(ll n)
{
	if(n <= 0) return 0;

	ll ret = 0;
	for(ll i = (n/4)*4; i <= n; i++) ret ^= i;
	return ret;
}

int main(void)
{
	ll A, B; cin >> A >> B;
	cout << (g(B) ^ g(A-1)) << endl;

	return 0;
}
