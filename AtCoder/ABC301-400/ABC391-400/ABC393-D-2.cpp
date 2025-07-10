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

// ABC393 https://atcoder.jp/contests/abc393

/*
 * メインの公式解説の方針で実装したもの。
 *   https://atcoder.jp/contests/abc393/editorial/12251
 *   https://x.com/kyopro_friends/status/1890759680395272463
 * 
 * 「0を1の間から追い出す」ことを考える。左と右どちらに追い出すのが特かを考えたい。
 * ある'0'について、そこから左にある'1'の個数をl, 右をrとする。
 * 自分以外にも'0'があり、適切に操作順を選ぶと
 *   ある'0'を追い出すときに、隣には1が並んでいる、
 *   つまり「左(右)にある1の個数と、追い出す操作回数が一致する」
 * にできる。
 * なので min(l,r) がこの'0'における操作回数。
 * 
 * 全'0'についてmin(l,r)を計算し、その総和が答。
**/

int main(void)
{
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	ll tot1 = count(s.begin(), s.end(), '1');  // 全体の1の個数

	ll ans = 0;
	ll cnt1 = 0;  // 左から見ていったとき、これまで登場した1の個数
	for(i = 0; i < N; i++)
	{
		if(s[i] == '0')
		{
			ll l = cnt1;
			ll r = tot1-cnt1;
			ans += min(l, r);
		}
		if(s[i] == '1') cnt1++;
	}
	cout << ans << endl;

	return 0;
}
