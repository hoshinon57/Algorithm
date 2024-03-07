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

// 競プロ典型90問:20 https://atcoder.jp/contests/typical90/tasks/typical90_t

/*
 * double型で愚直に計算すると誤差でWAになるため、
 * 整数比較に置き換えて判定する。
 * 
 * b*log2c=c^bなので、a<c^bかどうかを判定すればよい。
 * b^cの最大は17^13で、これはlong long型の範囲内に収まる。
 */

int main(void)
{
	ll a, b, c; cin >> a >> b >> c;
	ll p = 1;
	for(int i = 0; i < b; i++)
	{
		p *= c;
	}
	YesNo(a < p);

	return 0;
}
