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

// ABC162 https://atcoder.jp/contests/abc162

/*
 * 動的計画法で解く。
 * 
 * 例えばN=5のとき、どういう組み合わせが可能かを考える。
 * o:選ぶ, -:選ばない として、例えば以下が考えられる。
 *   o-o--
 *   o--o-
 *   o---o
 *   --o-o
 *   -o-o-
 * 証明はできなかったが、各(i-1)およびi文字目が "--" となるような数が必ず2つになりそう、ということが分かる。
 * （このとき、先頭と末尾に"-"を付与しておく）
 * 
 * Nが偶数のときは、これが必ず1つ。
 * Nを+1し、末尾に-INFを加えておくことで、最後の要素を選ばずに奇数にできる。
 * 
 * dp[i][j][k]を以下のように定義する。
 *   i個目まで見て、  (i=1～N)
 *   i個目を選んだかどうか  j=0:選ばない, j=1:選ぶ
 *   iと(i-1)個目をともに選ばない を行った回数  (k=0～2)
 *   のときの和の最大値
 * 最後のN要素目を選ぶか選ばないかをそれぞれ選び、
 *   dp[N][0][1] と dp[N][1][2]
 * のうち大きい方が答。
 */

int main(void)
{
	int i, k;
	int N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}  // a[0]は番兵
	if(N%2 == 0)  // 偶数は奇数にし、末尾に-INFを入れて選ばれないようにしておく
	{
		N++;
		a.push_back(-INF64);
	}
	
	vector dp(N+2, vector(1+1, vector<ll>(2+1, -INF64)));
	dp[0][0][0] = 0;
	for(i = 1; i <= N; i++)
	{
		for(k = 0; k <= 2; k++)
		{
			// a[i]を選ばないケース
			// a[i-1]も選ばないケースと、a[i-1]は選んだケースの2通り
			ll tmp = -INF64;
			if(k-1 >= 0)
			{
				tmp = dp[i-1][0][k-1];  // 連続して選ばない、のカウントが+1される
			}
			dp[i][0][k] = max(tmp, dp[i-1][1][k]);

			// a[i]を選ぶ
			// 連続して選ばない、のカウントは変化無し
			dp[i][1][k] = dp[i-1][0][k] + a[i];
		}
	}

	ll ans = max(dp[N][0][1], dp[N][1][2]);
	cout << ans << endl;

	return 0;
}
