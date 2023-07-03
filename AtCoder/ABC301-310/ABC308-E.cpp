#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC308 https://atcoder.jp/contests/abc308

/*
 * 動的計画法(もらうDP)で解く。
 * 
 * dp[i][j][k]を以下のように定義する。
 *   文字列Sのi文字目まで見て、  (i=0～N)
 *   "MEX"のj文字目まで一致させたときに、  (j=0～3)
 *   選んだ文字番号xに対するAxの集合を、0～2をそれぞれbit0～bit2に割り当てたときの、  (k=0～7)
 *   選び方の総和
 * 
 * DPの遷移は以下でになり、(1)(2)の和がdp[i][j][k]となる。
 * (1)i文字目を使わないケース：
 *   dp[i-1][j][k] を使う。
 * (2)i文字目を使うケース：
 *   そもそもS[i]が"MEX"のj文字目と不一致であれば、選べない。
 *   kに、A[i]に対応するbitが立っていなければ、同じく選べない。
 *   dp[i-1][j-1][(kからa[i])に対応するbitを落としたもの)] + dp[i-1][j-1][k] を使う。
 * 
 * dp[N][3][*]に対応するMEXの総和が答。
 */

ll dp[200005][4][8] = {0};

// 0をbit0, 1をbit1, 2をbit2に割り当てたときに、変数bitのmexを返す
// 例：{0,2}のときbit=5であり、戻り値は1
int f_mex(int bit)
{
	// bit:000 -> 0
	// bit:001 -> 1
	// bit:010 -> 0
	// bit:011 -> 2  以下略
	int ret[8] = {0, 1, 0, 2, 0, 1, 0, 3};
	return ret[bit];
};

int main(void)
{
	int i, j, k;
	ll N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	string s; cin >> s;
	s = '_' + s;
	string mex = "_MEX";

	dp[0][0][0] = 1;
	// もらうDP
	for(i = 1; i <= N; i++)  // i文字目に
	{
		dp[i][0][0] = dp[i-1][0][0];  // =1
		for(j = 1; j < 4; j++)  // "MEX"のj文字目を採用する
		{
			for(k = 0; k < 8; k++)
			{
				// i文字目を使わないケース
				dp[i][j][k] = dp[i-1][j][k];

				// i文字目を使うケース
				if(s[i] != mex[j]) continue;  // そもそも選べない
				if( (k&(1<<a[i])) == 0) continue;  // kに、a[i]に対応するbitが立っていない

				// a[i]に対応するbitが、
				ll d1 = dp[i-1][j-1][(k& (~(1<<a[i])) )];  // 立っていない部分からの遷移
				ll d2 = dp[i-1][j-1][k];  // 立っている部分からの遷移
				dp[i][j][k] += d1 + d2;
			}
		}
	}

	ll ans = 0;
	// dp[N][3][*] に対応するMEXの総和
	for(i = 0; i < 8; i++)
	{
		ans += dp[N][3][i] * f_mex(i);
	}
	cout << ans << endl;

	return 0;
}
