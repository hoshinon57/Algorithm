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

// ABC154 https://atcoder.jp/contests/abc154

/*
 * 桁DPで解く。
 * 
 * dp[i][smaller][j]を以下のように定義する。
 *   上からi桁目まで見たときに、  (i=1～)
 *   そこまでの値が、s未満である:smaller=1, sと等しい:smaller=0 であり、
 *   0でない数字をj個使っている  (j=0～K)
 *   ような数の種類数
 * 
 * 桁DPにて以下3パターンの遷移を考える。
 * (1)smaller=1 -> smaller=1
 * (2)smaller=0 -> smaller=0
 * (3)smaller=0 -> smaller=1
 * 
 * (1)(2)の具体的な遷移は実装を参照。
 * 
 * (3)について、i桁目の値をdとすると、使える数字は0～(d-1)のd種類。
 * このsmallerの遷移でd=0はありえないため、事前に除外しておく。
 *   0を使う場合はdp[i-1][0][j]から1通り(0使用)
 *   非0を使う場合はdp[i-1][0][j-1]から(d-1)通り(1～d-1を使用)
 * 
 * dp[L][0][K]+dp[L][1][K]が答。
 */

ll dp[105][2][4];

int main(void)
{
	int i, j;
	string s; cin >> s;
	int K; cin >> K;
	int L = (int)s.size();
	dp[0][0][0] = 1;
	for(i = 1; i <= L; i++)
	{
		int d = s[i-1] - '0';
		// smaller=1 -> smaller=1 への遷移
		for(j = 0; j <= K; j++)
		{
			dp[i][1][j] += dp[i-1][1][j];  // 0を使う
			if(j-1 >= 0) dp[i][1][j] += dp[i-1][1][j-1] * 9;  // 0以外を使う
		}

		// smaller=0 -> smaller=0 への遷移
		for(j = 0; j <= K; j++)
		{
			if(d == 0)  // 0を使う
			{
				dp[i][0][j] += dp[i-1][0][j];
			}
			else  // 0以外を使う
			{
				if(j-1 >= 0) dp[i][0][j] += dp[i-1][0][j-1];
			}
		}

		// smaller=0 -> smaller=1 への遷移
		// 使える数字は0～(d-1) のd種類
		if(d == 0) continue;  // このsmallerの遷移でd=0はありえない
		for(j = 0; j <= K; j++)
		{
			dp[i][1][j] += dp[i-1][0][j];  // 0を使う
			if(j-1 >= 0) dp[i][1][j] += dp[i-1][0][j-1] * (d-1);  // 1～(d-1)を使う
		}
	}

	cout << dp[L][0][K] + dp[L][1][K] << endl;

	return 0;
}
