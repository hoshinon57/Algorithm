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

// ABC411 https://atcoder.jp/contests/abc411

/*
 * 操作ごとの変化に着目する。
 *   https://x.com/kyopro_friends/status/1936425322079248672
 * 
 * クエリによって反転させるマスの左右を見て、黒マスの数を数える。
 * クエリのマスが白のとき、上記黒マスの数が
 *   0のとき: 領域が1増える
 *   1のとき: 領域数は変わらない
 *   2のとき: 領域が1減る
 * となる。
 * クエリマスが黒の場合も同様。
 * 
 * a[]の先頭末尾に番兵を入れておくと実装が楽。(番兵は白マスとする)
**/

int main(void)
{
	// 1-indexed
	ll N, Q; cin >> N >> Q;
	vector<ll> a(N+2);  // 0:白 1:黒  先頭末尾は白で番兵
	ll ans = 0;
	while(Q > 0)
	{
		Q--;
		ll x; cin >> x;
		ll cnt = a[x-1]+a[x+1];  // 左右の黒カウント
		if(a[x] == 0)
		{
			if(cnt == 0) ans++;
			if(cnt == 2) ans--;
		}
		else
		{
			if(cnt == 0) ans--;
			if(cnt == 2) ans++;
		}
		a[x] = 1-a[x];
		cout << ans << endl;
	}

	return 0;
}
