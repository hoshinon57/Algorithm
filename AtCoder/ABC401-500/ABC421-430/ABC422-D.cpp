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

// ABC422 https://atcoder.jp/contests/abc422

/*
 * 構築問題。s=2^Nとする。
 * 
 * まず、Kがsで割り切れる場合、Aの各要素をK/sにすれば良く、アンバランスさを0にできる。
 * 
 * Kがsで割り切れない場合、Kをsで割った余りを振り分ける必要がある。
 * このとき先頭から順に振り分けていくとNG.
 * ⇒N=3(s=8), K=30のとき、{4,4,4,4,4,4,3,3}だとアンバランスさが2になるが、
 * {4,4,4,3,4,4,4,3}であれば1を達成できる。
 * 
 * 余りを左側と右側に半々ずつ分けると、上手くいく。
 * 再帰でやると実装が楽。
 * ※@paruma184氏のツイートを参考に、セグ木の要素をイメージすると解きやすそう。
 *   https://x.com/paruma184/status/1964573703490224636
**/

int main(void)
{
	ll i;
	ll N, K; cin >> N >> K;
	ll s = 1LL<<N;  // 2^N
	vector<ll> ans(s, K/s);
	ll x = K%s;

	// [l,r) に余りxxを振り分ける
	auto f = [&](auto &self, ll l, ll r, ll xx) -> void
	{
		if(r-l == 1)
		{
			ans[l] += xx;
			return;
		}
		ll m = (l+r)/2;
		self(self, l, m, xx/2);
		self(self, m, r, xx-(xx/2));
	};
	f(f, 0, s, x);

	if(x == 0) cout << 0 << endl;
	else cout << 1 << endl;

	int sz_ = (int)ans.size();
	// cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
