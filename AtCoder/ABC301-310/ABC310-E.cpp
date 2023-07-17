#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC310 https://atcoder.jp/contests/abc310

/*
 * 動的計画法で解く。
 * 
 * まず、求めたいものを以下のように読み替える。
 *   i=1～Nそれぞれについて、
 *   f((1～iのそれぞれ), i) の和
 * 右端を固定するイメージ。
 * 
 * dp[i][j]を以下のように定義する。
 *   i文字目まで見たときに、  (i=0～N-1)
 *   f((1～iのそれぞれ), i) がjになるような  (j=0,1)
 *   個数
 * すると答はdp[0～N-1][1]の総和となる。
 * 
 * DPの遷移は以下となる。
 * (1)i文字目が0のとき
 *   1～i-1文字目までのNAND結果が0,1のどちらであっても、i文字目とNANDを取ると1になる。
 *   よってdp[i][1] = dp[i-1][0]+dp[i-1]となる。
 *   また、f(i,i)のぶんでdp[i][0]=1となる。
 * (2)i文字目が1のとき
 *   1～i-1文字目までのNAND結果により、i文字目とのNAND結果が決まる。
 *   -> 0 NAND 1 = 1, 1 NAND 1 = 0
 *   よってdp[i][0] = dp[i-1][1], dp[i][1] = dp[i-1][0] となる。
 *   これにf(i,i)のぶんでdp[i][1]に1を加える。
 */

int main(void)
{
	int N; cin >> N;
	string s; cin >> s;
	int i;
	vector<int> a(N); for(i = 0; i < N; i++)
	{
		if(s[i] =='1') a[i] = 1;
	}

	vector<vector<ll>> dp(N, vector<ll>(2, 0));
	// DP初期値
	if(a[0] == 0) dp[0][0] = 1;
	else dp[0][1] = 1;

	for(i = 1; i < N; i++)
	{
		// f(i,i)のぶんを計算
		if(a[i] == 0) dp[i][0] = 1;
		else dp[i][1] = 1;

		if(a[i] == 0)
		{
			dp[i][1] += dp[i-1][0] + dp[i-1][1];
		}
		else
		{
			dp[i][0] += dp[i-1][1];
			dp[i][1] += dp[i-1][0];
		}
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += dp[i][1];
	}
	cout << ans << endl;
	
	return 0;
}
