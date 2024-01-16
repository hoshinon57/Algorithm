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

// ABC336 https://atcoder.jp/contests/abc336

/*
 * コンテスト中に何とか解けたものの大苦戦した。
 * 
 * ピラミッドの左側と右側で分けて考える。
 * 左側について考えたときに、ある位置iについて「何段目の高さにまで可能か？」をdp_l[i]とする。
 * すると dp_l[i] = min(dp_l[i-1]+1, a[i]) として計算できる。
 * a[]の先頭末尾に0の番兵を入れておくと処理が楽。
 * 
 * 右側も同様。reverse()することで同じ処理でできる。
 * 
 * min(dp_l[i],dp_r[i])が、位置iをピラミッドの頂点としたときの高さの最大。
 * これをi=1～Nそれぞれについて求め、その最大値が答となる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・左右それぞれから見ていって、それぞれの結果を合わせるという考え方。(累積和にも似たような問題があった)
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N+2);  // 先頭と末尾に値0の番兵
	for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<int> dp_l(N+2), dp_r(N+2);  // reverseするのでN個の要素が中央に来るように
	for(i = 1; i <= N; i++)
	{
		dp_l[i] = min(dp_l[i-1]+1, a[i]);
	}
	reverse(a.begin(), a.end());
	for(i = 1; i <= N; i++)
	{
		dp_r[i] = min(dp_r[i-1]+1, a[i]);
	}
	reverse(dp_r.begin(), dp_r.end());

	int ans = 0;
	for(i = 1; i <= N; i++)
	{
		chmax(ans, min(dp_l[i], dp_r[i]));
	}
	cout << ans << endl;

	return 0;
}
