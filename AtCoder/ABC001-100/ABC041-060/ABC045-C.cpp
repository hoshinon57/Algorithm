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

// ABC045 https://atcoder.jp/contests/abc045

// +記号を入れる位置をbit全探索。
// 実装が意外と悩ましい。

int main(void)
{
	int i, j;
	string s; cin >> s;
	int len = (int)s.size();

	ll ans = 0;
	for(i = 0; i < (1<<(len-1)); i++)  // bitが1のところに+を入れる
	{
		ll now = (s[0]-'0');
		for(j = 0; j < len-1; j++)
		{
			if( ((i>>j)&1) != 0)  // +記号があれば、そこで一旦打ち切り
			{
				ans += now;
				now = 0;
			}
			now = now*10 + (s[j+1]-'0');
		}
		ans += now;  // 末尾の端数のぶんを加算必要
	}
	cout << ans << endl;

	return 0;
}
