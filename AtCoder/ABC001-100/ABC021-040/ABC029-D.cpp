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

// ABC029 https://atcoder.jp/contests/abc029

/*
 * 桁DP(Digit DP)で解く。
 * 
 * dp[i][smaller][j]を以下のように定義する。
 *   上からi桁目まで見たときに、  (i=1～)
 *   そこまでの値が、N未満である:smaller=1, Nと等しい:smaller=0 のときに、
 *   1をj個含むような値  (j=0～)
 *   の種類数
 * 
 * 桁DPにて以下3パターンの遷移を考える。
 * (1)smaller=1 -> smaller=1
 * (2)smaller=0 -> smaller=0
 * (3)smaller=0 -> smaller=1
 * 
 * (1)は1つ前の桁から、1を使うのが1パターン。1以外を使うのが9パターン。
 * 前者は[j-1], 後者は[j]から遷移。
 * 
 * (2)はi桁目の値をdとすると、
 * d =1のとき：1を使うので、[j-1]からの遷移が1パターン
 * d!=1のとき：1以外を使うので、[j]からの遷移が1パターン
 * 
 * (3)は同じくi桁目の値をdとすると、ここで使えるのは0～(d-1)であるので、
 * d=1のとき：使えるのは0のみ
 * d>=2のとき：1を使うのが1パターン、1以外を使うのが(d-1)パターン
 * d=0のとき：遷移無し
 * 
 * 答はNの桁数をLとして、
 * Σ(dp[L][0][k]*k + dp[L][1][k]*k) となる。
 */

ll dp[11][2][11];

int main(void)
{
	int i, j;
	string s; cin >> s;
	int L = (int)s.size();
	dp[0][0][0] = 1;
	for(i = 1; i <= L; i++)
	{
		int d = s[i-1]-'0';  // i文字目の値
		for(j = 0; j <= L; j++)
		{
			// smaller=1 -> smaller=1 への遷移
			if(j-1 >= 0) dp[i][1][j] += dp[i-1][1][j-1]; // 1を使う
			dp[i][1][j] += dp[i-1][1][j] * 9;  // 1以外を使う

			// smaller=0 -> smaller=0 への遷移
			if(d == 1)
			{
				if(j-1 >= 0) dp[i][0][j] += dp[i-1][0][j-1];
			}
			else
			{
				dp[i][0][j] += dp[i-1][0][j];
			}

			// smaller=0 -> smaller=1 への遷移
			// ※ACした後に気づいたが、if/elseで処理をまとめられる
			if(d == 1)
			{
				// 使えるのは0のみ
				dp[i][1][j] += dp[i-1][0][j];
			}
			else if(d >= 2)
			{
				if(j-1 >= 0) dp[i][1][j] += dp[i-1][0][j-1];  // 1を使う
				dp[i][1][j] += dp[i-1][0][j] * (d-1);  // 1以外を使う
			}
		}
	}

	ll ans = 0;
	// dp[L][0,1][k]*k
	for(i = 1; i <= 9; i++)
	{
		ans += dp[L][0][i]*i;
		ans += dp[L][1][i]*i;
	}
	cout << ans << endl;

	return 0;
}
