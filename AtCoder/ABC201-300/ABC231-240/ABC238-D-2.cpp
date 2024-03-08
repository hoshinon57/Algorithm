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

// ABC238 https://atcoder.jp/contests/abc238

/*
 * 公式解説と同じ方針で解いた。
 * (2年前はBFSをやったが、久しぶりに問題を見たらこの解法を思いついた)
 * https://atcoder.jp/contests/abc238/editorial/3359
 * 
 * x&y=a
 * x+y=s
 * aのbitが立っている部分は、x,yでも共にbitが1.
 * よって X=s-2*a とすると(addの残りのイメージ)、
 * Xにてbitが立っている部分は「x,yのどちらか片方だけのbitが1」である必要がある。
 * 
 * X&aが非0であればNG. X&aでbitが立っている部分はx,yともに1である必要があるため(x&y=aより)。
 * X<0の場合もNG.
 * それ以外はOK.
 */

bool solve(void)
{
	ll a, s; cin >> a >> s;
	ll x = s - a*2;
	if(x < 0) return false;
	if((x&a) != 0) return false;

	return true;
}

int main(void)
{
	int T; cin >> T;
	for(int i = 0; i < T; i++)
	{
		YesNo(solve());
	}

	return 0;
}
