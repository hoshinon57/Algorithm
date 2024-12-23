#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC384 https://atcoder.jp/contests/abc384

/*
 * tot=ΣAとして、S%=totとしてしまって良い。
 * 2周して考えて、先頭からの累積和をつど取っていき、sum-Sがこれまでの累積和に含まれていればYesとなる。
 * これは累積和をsetで管理していけばよい。
 * 
 * また、尺取り法でO(N)で解く実装もやってみた。#ifのelse側。
 * [l,r)の区間和がS未満の間、区間を伸ばしていく方針。
 * setを使う実装が153msに対し、こちらは56msだった。
 */

bool solve(void)
{
	ll i;
	ll N, S; cin >> N >> S;
	ll tot = 0;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i]; tot+=a[i];}
	S %= tot;

#if 1
	ll sum = 0;
	unordered_set<ll> st; st.insert(0);
	for(i = 0; i < N*2; i++)  // 2周する
	{
		sum += a[i%N];
		st.insert(sum);
		if(st.count(sum-S) > 0) return true;
	}
#else
	ll l, r;  // [l,r)
	r = 0;
	ll sum = 0;
	for(l = 0; l < N*2; l++)
	{
		while(r < N*2 && sum < S)
		{
			sum += a[r%N];
			r++;
		}
		if(sum == S) return true;
		if(l == r) r++;
		else sum -= a[l%N];
	}
#endif
	return false;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
