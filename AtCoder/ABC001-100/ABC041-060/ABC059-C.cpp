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

// ABC059 https://atcoder.jp/contests/abc059

/*
 * Greedyっぽいやつ。
 *   https://img.atcoder.jp/arc072/editorial.pdf
 *   https://drken1215.hatenablog.com/entry/2024/10/17/011315
 * 
 * まず、先頭から見ていって和が 正,負,正,負,... の順にするケースを考える。
 * 和を正にしたいケースにて、
 *   1)そこまでの和が負である
 *     ⇒和が1になるまで、つまり条件を満たす最小限で、操作する
 *       (和を2,3,...にするケースがあったとして、和を1としても、損はしない。どうせ後で操作すれば良いので)
 *   2)そこまでの和が正である
 *     ⇒操作不要
 * となる。
 * 和を負にしたいケースも同様。
 * 
 * 先頭から見ていって和が 負,正,負,正,... のケースは、
 * 全要素を-1倍して同じことをやれば良い。[実装を楽にする典型]
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = INF64;
	for(int _ = 0; _ < 2; _++)
	{
		ll sum = 0;
		ll cnt = 0;  // 操作回数
		for(i = 0; i < N; i++)
		{
			sum += a[i];
			if(i%2 == 0)  // 正にする
			{
				if(sum <= 0)
				{
					// 和を1にする
					cnt += 1-sum;
					sum = 1;
				}
			}
			else
			{
				if(sum >= 0)
				{
					cnt += sum+1;
					sum = -1;
				}
			}
		}
		chmin(ans, cnt);
		for(auto &e : a) e = -e;
	}
	cout << ans << endl;

	return 0;
}
