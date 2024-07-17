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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC159 https://atcoder.jp/contests/abc159

/*
 * 自力で解けず、解説を見た。
 * 主客転倒で考える。
 *   https://motsu-xe.hatenablog.com/entry/2020/03/22/224007
 *   https://drken1215.hatenablog.com/entry/2020/03/22/224200
 *   https://x.com/kyopro_friends/status/1241726260025278464
 * 
 * 以下、1-indexedで考える。
 * 和がSとなるようなある組み合わせにて、その最も狭い区間を[L1,R1]とする。
 * この区間を含むようなf(L,R)の個数は、L1から左側およびR1から右側へ伸ばせるので
 *   L1*(N-R1+1) 通り
 * となる。
 * 
 * すると、普通のナップザックDPをしつつ、
 *   和が0から増えるタイミング：i+1を加算
 *   和がSになるタイミング：(N-i+1)を積算
 * すると上手くいく。
 * 
 * dp[i][j]の定義は
 *   i個目まで見て、 (i=1～N)
 *   和がjになるような、 (j=0～S)
 *   組み合わせで、その区間を全て包含するような組み合わせの個数
 * となる感じ。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・主客転倒っぽいのは分かった。
 * ・ある区間の和がSになるなら、それを左右に伸ばした区間も和がSになる。
 *   つまり「最初に要素を選ぶとき」「最後に選ぶとき」に特別な処理をしてあげればよさそう。
 *   これを状態を持つDPということで耳DPと呼んだりもするらしい。
 */

int main(void)
{
	ll i, j;
	ll N, S; cin >> N >> S;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<vector<ll>> dp(N+1, vector<ll>(S+1, 0));  // dp[N+1][S+1]
	for(i = 0; i <= N; i++) dp[i][0] = i+1;  // DP初期値

	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= S; j++)
		{
			dp[i][j] = dp[i-1][j];
			if(j != S)
			{
				if(j-a[i] >= 0) dp[i][j] += dp[i-1][j-a[i]];
			}
			else
			{
				if(j-a[i] >= 0) dp[i][j] += dp[i-1][j-a[i]] * (N-i+1);
			}
			dp[i][j] %= MOD;
		}
	}
	cout << dp[N][S] << endl;

	return 0;
}
