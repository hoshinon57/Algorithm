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

// ABC369 https://atcoder.jp/contests/abc369

// 尺取り法で、ABC369-C.cppと違って差分配列を作らない版
// while部分で2要素の差分を左端と右端とで比較する

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = 0;
	ll l, r;
	r = 1;
	for(l = 0; l < N; l++)  // [l,r)
	{
		while(r < N && a[l+1]-a[l] == a[r]-a[r-1]) r++;
		ans += r-l;
		if(l+1==r) r++;  // 本問ではtrueにならないが、お作法的な
	}
	cout << ans << endl;

	return 0;
}
