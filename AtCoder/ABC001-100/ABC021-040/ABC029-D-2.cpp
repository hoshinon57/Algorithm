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

// ABC029 https://atcoder.jp/contests/abc029

/*
 * 実装方針を変えて再解きした版。
 * dp[i][sm][j]を
 *   i桁目まで見て、 (i=1～L)
 *   sm:未満フラグ (0:同じ, 1:未満)
 *   1がj個含まれる整数の、 (j=0～9)
 *   種類数
 * として定義する。
 * DP遷移は実装、およびdp-digitDP.cppを参照。
 * 実装では[i]はdp,ndpの2つで表して次元数を減らしている。
 * 
 * 答はdp[*][j]*jの総和。
 */

int main(void)
{
	string s; cin >> s;
	int L = (int)s.size();
	const int MK = 11;
	vector<vector<ll>> dp(2, vector<ll>(MK, 0));
	dp[0][0] = 1;

	// 配るDP
	int i, sm, j;
	for(i = 0; i < L; i++)
	{
		vector<vector<ll>> ndp(2, vector<ll>(MK, 0));
		for(sm = 0; sm < 2; sm++)
		{
			for(j = 0; j < MK-1; j++)  // dp[sm][j]から配る
			{
				const int D = s[i] - '0';   // 配る先の値
				int ed = (sm ? 9 : D);
				for(int d = 0; d <= ed; d++)  // 次に使う値
				{
					int nsm = (sm || d < D);  // 配る元が"未満", もしくは配り先の値がD未満なら、smaller=1.
					int nj = j;
					if(d == 1) nj++;  // 次に使う値が1なら、nj=j+1とする
					ndp[nsm][nj] += dp[sm][j];
				}
			}
		}
		swap(dp, ndp);
	}

	ll ans = 0;
	for(sm = 0; sm < 2; sm++)
	{
		for(j = 0; j < MK; j++)
		{
			ans += dp[sm][j] * j;
		}
	}
	cout << ans << endl;

	return 0;
}
