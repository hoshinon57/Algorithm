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

/*
 * 実装方針を変えて再解きした版。
 * dp[i][sm][j]を
 *   i桁目まで見て、 (i=1～L)
 *   sm:未満フラグ (0:同じ, 1:未満)
 *   ここまで選んだ値の中に、禁止された数字が j=0:ない,j=1:ある ときの、
 *   数字の種類数
 * として定義する。
 * DP遷移は実装、およびdp-digitDP.cppを参照。
 * 
 * [0,x]の範囲の禁止された数字を返す関数をfunc(x)とすると、答はfunc(b)-func(a-1).
 * func(x)はdp[L][0][1]+dp[L][1][1].
 */

ll func(ll x)
{
	string s = to_string(x);
	int L = (int)s.size();
	vector dp(L+1, vector(2, vector<ll>(2, 0)));
	dp[0][0][0] = 1;

	int i, sm, j;
	// 配るDP
	for(i = 0; i < L; i++)
	{
		for(sm = 0; sm < 2; sm++)
		{
			for(j = 0; j < 2; j++)
			{
				const int D = s[i]-'0';  // 配る先の値
				int ed = (sm ? 9 : D);
				for(int d = 0; d <= ed; d++)  // 次に使う値
				{
					int nsm = (sm || d < D);
					int nj = (j || d == 4 || d == 9);
					dp[i+1][nsm][nj] += dp[i][sm][j];
				}
			}
		}
	}

	return dp[L][0][1]+dp[L][1][1];
}

int main(void)
{
	ll a, b; cin >> a >> b;
	cout << func(b) - func(a-1) << endl;

	return 0;
}
