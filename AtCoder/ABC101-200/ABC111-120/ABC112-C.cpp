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

// ABC112 https://atcoder.jp/contests/abc112

/*
 * Cx,Cyについて全探索。
 * N個の情報それぞれについて矛盾が無ければOK.
 * Cx,Cyともに0～100で、N=100なので、10^6で間に合う。
 * 
 * …と実装してWA.
 * 制約の max(***, 0) を見逃していた。
 * h[i]が0のとき、復元した高さtmpは、現在の予想の高さnh以上になるケースがある。
 * よってh[i]=0なら tmp<nh のとき矛盾となる。
 * (h[i]!=0なら tmp!=nh で矛盾)
 * ※公式解説を見ると、どうもh[i]=0のときは判定をそもそも無視してよいっぽい…？
 * 
 * とすると、現在の予想の高さnhは最初の情報ではなく、h[i]!=0である情報から作る必要がある。
 * 制約よりh[i]!=0である情報は必ずあるっぽいので、それを使う。
 * 
 * 参考記事：
 *   https://drken1215.hatenablog.com/entry/2018/11/06/120500
 *   https://betrue12.hateblo.jp/entry/2018/10/06/224032
 *   https://scrapbox.io/procon-kirokuyou/ABC112_C_-_Pyramid_(300)
 *   https://blog.hamayanhamayan.com/entry/2018/10/06/233650
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> x(N), y(N), h(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i] >> h[i];}

	for(ll cx = 0; cx <= 100; cx++)
	{
		for(ll cy = 0; cy <= 100; cy++)
		{
			ll nh;  // 高さの予想
			// h!=0の情報を元に、高さの予想を求める
			for(i = 0; i < N; i++)
			{
				if(h[i] == 0) continue;
				nh = abs(cx-x[i]) + abs(cy-y[i]) + h[i];
				break;
			}
			bool ok = true;
			for(i = 0; i < N; i++)
			{
				ll tmp = abs(cx-x[i]) + abs(cy-y[i]) + h[i];
				if(h[i] != 0)
				{
					if(tmp != nh) ok = false;
				}
				else
				{
					// tmp >= nhならOK
					if(tmp < nh) ok = false;
				}
			}
			if(ok)
			{
				cout << cx << " " << cy << " " << nh << endl;
				return 0;
			}
		}
	}

	return 0;
}
