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

/*
 * 尺取り法で解く。
 * ad[]をa[]の差分とし(要素数はN-1となる)、
 * 各lについてad[]が[l,r)の範囲で等しいとき、(r-l)を答に加算していく。
 * 
 * 元のa[]にて長さ1の数列がカウントされないため、最後に+Nする。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> ad;
	for(i = 1; i < N; i++) ad.push_back(a[i]-a[i-1]);  // 要素数(N-1)個

	ll ans = 0;
	int l, r;
	r = 0;
	for(l = 0; l < N-1; l++)  // [l,r)  要素数はN-1
	{
		while(r < N-1 && ad[l] == ad[r]) r++;
		ans += r-l;
		if(l==r) r++;  // 本問ではtrueにならないが
	}
	cout << ans+N << endl;

	return 0;
}
