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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC129 https://atcoder.jp/contests/abc129

/*
 * 実装方針を変えて再解きした版。
 * dp[i][sm]を
 *   i桁目まで見て、 (i=1～L)
 *   sm:未満フラグのときの、 (0:同じ, 1:未満)
 *   組み合わせの数
 * として定義する。
 * DP遷移は実装、およびdp-digitDP.cppを参照。
 * 実装では[i]はdp,ndpの2つで表して次元数を減らしている。
 * 
 * 答はdp[sm=0]+dp[sm=1].
 */

int main(void)
{
	string s; cin >> s;
	int L = (int)s.size();

	vector<mint> dp(2);
	dp[0] = 1;

	// 配るDP
	int i, sm;
	for(i = 0; i < L; i++)
	{
		vector<mint> ndp(2);
		for(sm = 0; sm < 2; sm++)
		{
			const int D = s[i] - '0';  // 配る先の値
			int ed = (sm ? 1 : D);
			for(int d = 0; d <= ed; d++)  // 次に使う値
			{
				int nsm = (sm || d < D);
				// d=0なら1倍, d=1なら2倍
				ndp[nsm] += dp[sm] * (d+1);
			}
		}
		swap(dp, ndp);
	}
	cout << (dp[0]+dp[1]).val() << endl;

	return 0;
}
