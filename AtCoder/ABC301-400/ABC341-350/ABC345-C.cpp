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

// ABC345 https://atcoder.jp/contests/abc345

/*
 * len=|S|とする。
 * i,jの大小関係をいったん無視すると、選び方はlen*len通り。
 * ここから「操作しても元の文字列と同じ選び方」を引けばよい。(※)
 * 
 * 元の文字列になるのは、i,jに同じ文字を選んだ場合となる。
 * よって各文字の登場回数をカウントしておき、
 *   cnt[i]*cnt[i]
 * を'a'～'z'まで計算して減算すればよい。
 * i<jという条件については、最後に2で割ればOK.
 * 
 * ただし(※)には1点考慮漏れがあり、同じ文字列も1回はカウントしてあげる必要がある。
 * cnt[i]>=2となる要素があれば、元と同じ文字列が作れるので、それをチェックしておく。
 */

int main(void)
{
	ll i;
	string s; cin >> s;
	ll len = s.size();
	vector<ll> cnt(26);
	for(auto &e : s) cnt[e-'a']++;
	ll ans = len*len;
	bool same = false;  // 操作後、元と同じ文字列が作れるならtrue
	for(i = 0; i < 26; i++)
	{
		if(cnt[i] >= 2) same = true;
		ans -= cnt[i]*cnt[i];
	}
	ans /= 2;
	if(same) ans++;
	cout << ans << endl;

	return 0;
}
