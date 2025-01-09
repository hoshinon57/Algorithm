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

// ABC154 https://atcoder.jp/contests/abc154

/*
 * 実装方針を変えて再解きした版。

 * dp[i][sm][j]を以下のように定義する。
 *   i桁目まで見て、 (i=1～L)
 *   sm:未満フラグ (0:同じ, 1:未満)
 *   0でない値がj個 (j=0～K)
 *   である整数の種類数
 * 
 * DP遷移は実装、およびdp-digitDP.cppを参照。
 * 実装では[i]はdp,ndpの2つで表して次元数を減らしている。
 * 
 * 答はdp[sm=0][K]+dp[sm=1][K].
 */

int main(void)
{
	string s; cin >> s;
	ll K; cin >> K;
	int L = (int)s.size();
	vector<vector<ll>> dp(2, vector<ll>(K+1, 0));  // dp[2][K+1]
	dp[0][0] = 1;

	int i, sm, j;
	for(i = 0; i < L; i++)
	{
		vector<vector<ll>> ndp(2, vector<ll>(K+1, 0));
		for(sm = 0; sm < 2; sm++)
		{
			for(j = 0; j < K+1; j++)
			{
				const int D = s[i] - '0';
				int ed = (sm ? 9 : D);
				for(int d = 0; d <= ed; d++)
				{
					int nsm = (sm || d < D);
					int nj = j;
					if(d != 0) nj++;
					if(nj > K) continue;  // K個を超えたら除外
					ndp[nsm][nj] += dp[sm][j];
				}
			}
		}
		swap(dp, ndp);
	}

	ll ans = 0;
	for(sm = 0; sm < 2; sm++)
	{
		ans += dp[sm][K];
	}
	cout << ans << endl;

	return 0;
}
