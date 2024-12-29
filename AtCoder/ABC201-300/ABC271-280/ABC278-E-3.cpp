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

// ABC278 https://atcoder.jp/contests/abc278

/*
 * 別解で解いてみた版。
 *   https://atcoder.jp/contests/abc278/editorial/5236
 * (別解と違い、0-indexedで自分は実装している)
 * 
 * 各値a(1～N)ごとに、その値が登場する座標のymin,ymax,xmin,xmaxを求める。
 * すると各クエリ[y,y+h) [x,x+w)について、
 *   y<=ymin かつ ymax<y+h かつ x<=xmin かつ xmax<x+w
 * であれば、その値は全て塗りつぶされる。
 * 
 * 計算量はO(HWN).
 * だが自作の二次元累積和ライブラリよりもずっと早かった。(38ms vs 1173ms)
 */

int main(void)
{
	// 0-indexed
	int i;
	int y, x;
	int H, W, N; cin >> H >> W >> N;
	int h, w; cin >> h >> w;
	vector<int> ymin(N+1, INF32), ymax(N+1, -INF32), xmin(N+1, INF32), xmax(N+1, -INF32);
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			int a; cin >> a;
			chmin(ymin[a], y);
			chmax(ymax[a], y);
			chmin(xmin[a], x);
			chmax(xmax[a], x);
		}
	}

	for(y = 0; y+h <= H; y++)
	{
		for(x = 0; x+w <= W; x++)  // [y,y+h), [x,x+w) の区間
		{
			int ans = N;
			for(i = 1; i <= N; i++)
			{
				if(y<=ymin[i] && ymax[i]<y+h && x<=xmin[i] && xmax[i]<x+w) ans--;  // 値iは塗りつぶされる
			}
			cout << ans;
			if(i != N) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
