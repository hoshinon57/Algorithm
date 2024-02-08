#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC216 https://atcoder.jp/contests/abc216

/*
 * 動的計画法で解く。
 * 
 * {A,B}をAの昇順にソートしておくと、
 *   max(A1～Aiまでの部分集合) <= Ai
 * となる。
 * もっと言うと
 *   max(A1～Aiまでの部分集合のうち、Aiを選ぶもの) == Ai
 * となる。
 * 
 * よって
 *   dp[i][j] = i個目まで見て、それまで選んだBの総和がjとなる組み合わせの数
 *   (1)Bを選ばない：dp[i][j] += dp[i-1][j]
 *   (2)Bを選ぶ    ：dp[i][j] += dp[i-1][j-B]
 * という「いつものDP」を考えたときに、
 * (2)にてA>=jであれば条件を満たすことが分かる。この数の総和が答。
 * 
 * [ACまでの思考の流れ]
 * ・集合の最大値が必要 ⇒ 昇順ソートしておけば、選んだAが常に集合の最大値となる。
 * ・「いつものDP」をやって、遷移時にΣBがA以下かどうかで判定できそう。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	vector<int> ord(N);
	iota(ord.begin(), ord.end(), 0);
	sort(ord.begin(), ord.end(), [&](int x, int y){ return a[x] < a[y];});
	const int ABMAX = 5000;
	vector<vector<ll>> dp(N+1, vector<ll>(ABMAX+1, 0));  // dp[N+1][ABMAX+1]
	dp[0][0] = 1;
	ll ans = 0;
	for(i = 1; i <= N; i++)
	{
		// ord[]の順にアクセス
		int aa = a[ord[i-1]];
		int bb = b[ord[i-1]];
		for(j = 0; j <= ABMAX; j++)
		{
			dp[i][j] = dp[i-1][j];
			if(j-bb >= 0)
			{
				dp[i][j] += dp[i-1][j-bb];
				if(aa >= j)
				{
					ans += dp[i-1][j-bb];
					ans %= MOD;
				}
			}
			dp[i][j] %= MOD;
		}
	}
	cout << ans << endl;

	return 0;
}
