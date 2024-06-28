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

// ABC359 https://atcoder.jp/contests/abc359

// evima氏の解説が近かった。
// https://atcoder.jp/contests/abc359/editorial/10278
// まずタイルの左側に寄せ、S->Tへ右側へ移動するようにswapしておく。
// Y方向の移動回数を計算し、それでX方向も合わせられるならそれが答。
// X方向の移動が足りなければ、残りを2で割ったものを加算すれば答。

int main(void)
{
	ll sx, sy, tx, ty; cin >> sx >> sy >> tx >> ty;
	// 左側に寄せる
	if((sx+sy)%2 == 1) sx--;
	if((tx+ty)%2 == 1) tx--;

	// S->Tへ、右方向に移動するものとする
	if(sx > tx)
	{
		swap(sx, tx);
		swap(sy, ty);
	}

	ll dy = abs(sy-ty);  // y方向の移動回数
	ll ans = dy;
	if(tx-sx > dy)
	{
		ans += ((tx-sx)-dy)/2;
	}
	else {;}  // X方向の移動は足りている
	cout << ans << endl;

	return 0;
}
