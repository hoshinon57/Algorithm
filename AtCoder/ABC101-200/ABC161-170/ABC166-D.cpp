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

// ABC166 https://atcoder.jp/contests/abc166

/*
 * ポイント：基本は全探索。(A,B)のペアを求めよ的な問題では、少なくとも片方は全探索なことが多そう。
 * 
 * A^5-B^5を因数分解して手でごちゃごちゃしてると、A,Bともに±10^3の範囲で探索すればよいのでは？となった。
 * その範囲で全探索したらAC.
 * 
 * 解法を見ると、1000^5-999^5≒5^12 なのでこの辺りで十分という考え方だった。なるほど。
 */

int main(void)
{
	ll a, b;
	ll x; cin >> x;
	for(a = -1000; a <= 1000; a++)
	{
		for(b = -1000; b <= 1000; b++)
		{
			if((a*a*a*a*a - b*b*b*b*b) == x)
			{
				cout << a << " " << b << endl;
				return 0;
			}
		}
	}

	return 0;
}
