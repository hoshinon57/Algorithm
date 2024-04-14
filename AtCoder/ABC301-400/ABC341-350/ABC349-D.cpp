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

// ABC349 https://atcoder.jp/contests/abc349

/*
 * 左から貪欲に決めていく。公式解説と同じ考え方。
 *   https://atcoder.jp/contests/abc349/editorial/9772
 * 
 * [l,r)のlについて、lを左端とする最大長を取ることを考えると、
 *   lが2^iで割り切れる
 *   l+2^i<=r である
 * を満たす最大のiとすればよい。
 * このとき、lをl+2^iへアップデートし、また同じ処理を行う。
 * l=rになったら終了。
 */

int main(void)
{
	ll i;
	ll l, r; cin >> l >> r;
	vector<pair<ll,ll>> ans;  // {l,r}
	while(l < r)
	{
		for(i = 60; i >= 0; i--)  // i=0では必ず条件を満たす
		{
			ll powi = 1LL<<i;
			// lが2^iで割り切れ、かつl+2^i<=である必要がある
			if(l%powi != 0) continue;
			if(l+powi > r) continue;

			ans.push_back({l, l+powi});
			l = l+powi;
			break;
		}
	}

	cout << ans.size() << endl;
	for(auto &e : ans)
	{
		cout << e.first << " " << e.second << endl;
	}

	return 0;
}
