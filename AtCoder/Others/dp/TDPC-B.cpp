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

// https://atcoder.jp/contests/tdpc/tasks/tdpc_game

/*
 * ゲームのDP.
 * drken氏の解説を元に実装した内容。
 *   https://drken1215.hatenablog.com/entry/2023/07/22/023939
 * 
 * dp[i][j]を以下のように定義する。
 *   左山の次に取るものの価値がa[i],
 *   右山がb[j],
 *   のときから始めたときに、先手番が取る得点の最大
 * 
 * 再帰関数の引数にsumとして「残っている価値の総和」を入れると、
 * 左山から取るとき、
 *   まずa[i]点ゲット
 *   残価値の総和は (sum-a[i]) であり、f(i+1, j, sum-a[i]) が相手の得点なので、
 *   (sum-a[i])-f(i+1, j, sum-a[i]) が以降の得点
 * となる。
 * 右山も同様。
**/

int main(void)
{
	int i;
	int A, B; cin >> A >> B;
	int tot = 0;
	vector<int> a(A); for(i = 0; i < A; i++) {cin >> a[i]; tot += a[i];}
	vector<int> b(B); for(i = 0; i < B; i++) {cin >> b[i]; tot += b[i];}
	vector<vector<int>> dp(A+1, vector<int>(B+1, -1));  // dp[A+1][B+1]

	auto f = [&](auto &self, int x, int y, int sum) -> int
	{
		auto &d = dp[x][y];
		if(d != -1) return d;

		d = 0;
		if(x != A)
		{
			int tmp = a[x];  // 今回取れる点数
			tmp += (sum-a[x]) - self(self, x+1, y, sum-a[x]);
			chmax(d, tmp);
		}
		if(y != B)
		{
			int tmp = b[y];
			tmp += (sum-b[y]) - self(self, x, y+1, sum-b[y]);
			chmax(d, tmp);
		}
		return d;
	};
	auto ans = f(f, 0, 0, tot);
	cout << ans << endl;

	return 0;
}
