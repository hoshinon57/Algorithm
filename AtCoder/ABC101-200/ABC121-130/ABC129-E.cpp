#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = (ll)(1e9+7);
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC129 https://atcoder.jp/contests/abc129

/*
 * 桁DPで解く。
 * 
 * a + b = a xor b
 * であることを考えると、加算にて繰り上がりが発生すると条件を満たせなさそう、というのが分かる。
 * つまり、各bitにて(a,b)=(0,0),(1,0),(0,1)のいずれかとなり、
 * bitごとに独立して考えることができる。
 * 
 * dp[i][smaller]を以下のように定義する。
 *   Lの上からi桁目まで見て、  (i:1～|L|)
 *   そこまでの値が Lと等しい：smaller=0, Lより小さい：smaller=1
 *   ときの組み合わせの数
 * 答は dp[|L|][0] + dp[|L|][1] となる。
 * 
 * Lで今見ているbitの値をdとする。
 * dが0であれば(a,b)=(0,0)であり、1であれば(0,1) or (1,0)となる。
 * それを踏まえ、DP遷移は以下のようになる。
 * (1)smaller=0 -> smaller=0
 *   d=0であれば(0,0)の1通り、d=1であれば(0,1),(1,0)の2通り。
 * (2)smaller=1 -> smaller=1
 *   (0,0),(0,1),(1,0)いずれもOK. 3通り。
 * (3)smaller=0 -> smaller=1
 *   d=1のときのみ可能な遷移。
 *   (0,0)のみの1通り。
 * 
 * dp[L][0]+dp[L][1]が答。
 */

int main(void)
{
	int i;
	string s; cin >> s;
	int L = (int)s.size();
	s = '*' + s;  // s[]とdp[]の添え字を合わせる
	vector<vector<ll>> dp(L+1, vector<ll>(2, 0));
	dp[0][0] = 1;

	// 配るDP
	for(i = 0; i < L; i++)
	{
		int d = (int)(s[i+1] - '0');  // i+1桁目の値
		// smaller 0->0
		if(d == 0)
		{
			dp[i+1][0] += dp[i][0];  // (a,b)=(0,0)
		}
		else
		{
			dp[i+1][0] += dp[i][0]*2;  // (a,b)=(0,1),(1,0)
		}

		// smaller 1->1
		dp[i+1][1] += dp[i][1]*3;  // (a,b)=(0,0),(0,1),(1,0)

		// smaller 0->1
		if(d == 1)
		{
			dp[i+1][1] += dp[i][0];  // i+1桁目が0のケース
		}

		dp[i+1][0] %= MOD;
		dp[i+1][1] %= MOD;
	}
	cout << (dp[L][0] + dp[L][1])%MOD << endl;

	return 0;
}
