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

// ABC070 https://atcoder.jp/contests/abc070

/*
 * ABC056-D.cppの以下について、累積和で実装した内容。
 * ＞"X=[K-ai-j,K-j) 内のdp_r[N-i][X]" の部分について、愚直にやるとTLEする。
 * ＞自分は区間加算のセグ木でまずACした。
 * ＞しかし累積和で十分だった…
 **/

int main(void)
{
	int i, j;
	int N, K; cin >> N >> K;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	auto f_dp = [&](vector<vector<int>> &dp) -> void
	{
		// もらうDP
		for(i = 1; i <= N; i++)
		{
			for(j = 0; j <= K; j++)
			{
				dp[i][j] = dp[i-1][j];
				// 組み合わせの場合の数でやると、オーバーフローしたのかWAだった
				// なので存在有無の0 or 1で実装した
//				if(j-a[i-1] >= 0) dp[i][j] += dp[i-1][j-a[i-1]];
				if(j-a[i-1] >= 0) dp[i][j] |= dp[i-1][j-a[i-1]];
			}
		}
	};
	vector<vector<int>> dpl(N+1, vector<int>(K+1, 0));
	dpl[0][0] = 1;
	f_dp(dpl);

	// 右から計算
	vector<vector<int>> dpr(N+1, vector<int>(K+1, 0));
	dpr[0][0] = 1;
	reverse(a.begin(), a.end());
	f_dp(dpr);
	reverse(a.begin(), a.end());

	int ans = 0;
	for(i = 0; i < N; i++)  // a[i]について判定
	{
		vector<int> sum(dpr[N-i-1].size()+1); for(int kk = 0; kk < (int)dpr[N-i-1].size(); kk++) {sum[kk+1] = sum[kk] + dpr[N-i-1][kk];}
		bool must = false;  // a[i]が必要ならtrue
		for(j = 0; j <= K; j++)  // 総和j
		{
			if(dpl[i][j] == 0) continue;

			int l = max(K-a[i]-j, 0);
			int r = K-j;  // [l,r)
			// [l,r)の和 -> sum[r]-sum[l]
			if(sum[r]-sum[l] > 0)
			{
				must = true;
				break;
			}
		}
		if(!must) ans++;
	}
	cout << ans << endl;

	return 0;
}
