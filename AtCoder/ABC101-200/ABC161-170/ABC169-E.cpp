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

// ABC169 https://atcoder.jp/contests/abc169

/*
 * 自力で解けず、解説を見た。
 *   https://blog.hamayanhamayan.com/entry/2020/06/01/210806
 *   https://scrapbox.io/pj-kyopro/%5BAtCoder_ABC_%23169%5D_E_-_Count_Median
 *   https://drken1215.hatenablog.com/entry/2020/06/02/104700
 * 
 * 結局のところ、
 *   作れる中央値の最小値
 *   作れる中央値の最大値
 * の範囲は全て作れる。なので、
 * ・Nが奇数：
 *   Aの中央値とBの中央値の間で、1単位で作れる。
 * ・Nが偶数：
 *   Aの中央値とBの中央値の間で、0.5単位で作れる。
 *   計算式としては ((A1+A2)/2 + (B1+B2)/2)*2 + 1 より
 *   (A1+A2) + (B1+B2) + 1 となる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・方針が分からないなら、手を動かして試してみる。これに尽きそう。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N), b(N);
	for(i = 0; i < N; i++) cin >> a[i] >> b[i];
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	int ans = 0;
	if(N%2 == 1)
	{
		int idx = N/2;
		ans = b[idx] - a[idx] + 1;
	}
	else
	{
		int idx = N/2-1;  // 中央値に使う2つの値のうち、先頭の方
		ans = (b[idx]+b[idx+1]) - (a[idx]+a[idx+1]) + 1;
	}
	cout << ans << endl;

	return 0;
}
