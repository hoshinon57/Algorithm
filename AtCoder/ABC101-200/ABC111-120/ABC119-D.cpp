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

// ABC119 https://atcoder.jp/contests/abc119

/*
 * クエリの位置xに対して
 *   x -> x以降にある神社 -> 前後の寺
 *   x -> xより手前にある神社 -> 前後の寺
 *   神社と寺を逆にして同様
 * のうち最短距離のものが答。
 * 
 * 神社、寺ともに±INFに2つずつ番兵をセットした。2個は不要だったかも。
 * 「神社と寺を逆にして」はswapで簡易化している。
**/

int main(void)
{
	ll i;
	ll A, B, Q; cin >> A >> B >> Q;
	vector<ll> s(A); for(i = 0; i < A; i++) {cin >> s[i];}
	vector<ll> t(B); for(i = 0; i < B; i++) {cin >> t[i];}

	s.push_back(-INF64); s.push_back(-INF64+1);
	s.push_back(INF64); s.push_back(INF64+1);
	sort(s.begin(), s.end());
	t.push_back(-INF64); t.push_back(-INF64+1);
	t.push_back(INF64); t.push_back(INF64+1);
	sort(t.begin(), t.end());

	while(Q > 0)
	{
		Q--;
		ll x; cin >> x;
		ll ans = INF64;

		// 位置posから、左右の寺への距離の短い方を返す
		auto cal = [&](ll pos) -> ll
		{
			auto itr = lower_bound(t.begin(), t.end(), pos);
			ll tmp = min((*itr-pos), (pos-*prev(itr)));
			return tmp;
		};
		for(int _ = 0; _ < 2; _++)
		{
			auto itr = lower_bound(s.begin(), s.end(), x);  // 右側の神社
			chmin(ans, (*itr-x) + cal(*itr));

			itr--;  // 左側の神社
			chmin(ans, (x-*itr) + cal(*itr));

			swap(s, t);
		}
		cout << ans << endl;
	}

	return 0;
}
