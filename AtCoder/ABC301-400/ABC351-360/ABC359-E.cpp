#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <stack>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC359 https://atcoder.jp/contests/abc359

/*
 * コンテスト時は別解法(セグ木と遅延セグ木)で解いたので、スタックを用いた解法を勉強して実装してみた。
 * 最大長方形問題のようにスタックを用いて解く。
 *   https://x.com/kyopro_friends/status/1804517516225728867
 *   https://www.youtube.com/watch?v=FGdRakXIEXM
 *   https://qiita.com/viral_8/items/30775c6cb65b58ab9ff6
 * 
 * 次に超えようとしている壁の高さをhiとする。
 * 手前の壁hjについて hj<hi であれば、そこもhiまで水が溜まることになる。
 * スタックで {高さh, その長さl} の形で保持していくと、これはhについて単調減少になる。
 * 
 * 各hiについてスタックの先頭hjを見て、hj<hiなら (hi-hj)*l だけ答に加算する。(スタックからも取り除く)
 * hi自身もスタックに積む。このとき上記の答に加算したぶんだけ、長さlも増やしておく。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> h(N); for(i = 0; i < N; i++) {cin >> h[i];}

	ll ans = 1;
	stack<pair<ll,ll>> st;  // {高さh, その長さl}
	for(i = 0; i < N; i++)  // h[i]
	{
		ll len = 1;  // h[i]の長さ
		while(!st.empty())
		{
			auto [hh, l] = st.top();
			if(hh > h[i]) break;

			ans += (h[i]-hh) * l;
			len += l;
			st.pop();
		}
		ans += h[i];
		st.push({h[i], len});

		if(i != 0) cout << " ";
		cout << ans;
	}
	cout << endl;

	return 0;
}
