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

// ABC038 https://atcoder.jp/contests/abc038

/*
 * 尺取り法で解く。実装を参照。
 * 参考：https://qiita.com/drken/items/ecd1a472d3a0e7db8dce
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = 0;
	int l, r;
	r = 1;
	for(l = 0; l < N; l++)  // [l,r)  少なくとも1つの要素を囲んでいるようにする
	{
		while(r < N && a[r-1] < a[r])  // rを1つ進めても、条件を満たすか
		{
			r++;
		}
		ans += r-l;
		if(r-l == 1) r++;  // lが増えると選択する要素数が0個になるので、rを1加算しておく
	}
	cout << ans << endl;

	return 0;
}
