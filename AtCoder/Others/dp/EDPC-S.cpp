#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 桁DPで解く。
 * 
 * dp[i][j][smaller]を以下のように定義する。
 *   i桁目まで見て、
 *   数字の総和%D がjであり、
 *   そこまでの値が、K未満である:smaller=1, Kと等しい:smaller=0
 *   ような値の種類数
 * 
 * 以下3パターンの遷移を考える。
 * (1)smaller=1 -> smaller=1
 * (2)smaller=0 -> smaller=0
 * (3)smaller=0 -> smaller=1
 * 
 * (1)は各dp[i][j]からk=0～9の10通りの値を使える。
 * [i][j]から[i+1][j+k]への10通りの遷移。  ※(j+k)%D が正しいが、記載省略。以下同様
 * 
 * (2)は(i+1)桁目の値をdとすると、dのみ使える。
 * [i][j]から[i+1][j+d]への遷移。
 * 
 * (3)は同じくi桁目の値をdとすると、
 * ここで使えるのはk=0～(d-1)のd通りであること以外は(1)と同様。
 * 
 * 整数0も答に含まれるが、条件は1以上であるため、1減算必要。
 * dp[L][0][0]+dp[L][0][1]-1 が答。
 */

ll dp[10005][105][2];

int main(void)
{
	int i, j, k;
	string s; cin >> s;
	int D; cin >> D;
	int L = s.size();
	dp[0][0][0] = 1;  // DP初期値

	// i->i+1への配るDP
	for(i = 0; i < L; i++)
	{
		int d = s[i] - '0';  // [i+1]の数値
		for(j = 0; j < D; j++)
		{
			int nxt;
			// smaller=1 -> smaller=1 への遷移
			for(k = 0; k < 10; k++)
			{
				nxt = (j+k)%D;
				dp[i+1][nxt][1] += dp[i][j][1];
				dp[i+1][nxt][1] %= MOD;
			}

			// smaller=0 -> smaller=0 への遷移
			// dのみ選べる
			nxt = (j+d)%D;
			dp[i+1][nxt][0] += dp[i][j][0];
			dp[i+1][nxt][0] %= MOD;

			// smaller=0 -> smaller=1 への遷移
			// 0～(d-1)のd通り選べる
			for(k = 0; k < d; k++)
			{
				nxt = (j+k)%D;
				dp[i+1][nxt][1] += dp[i][j][0];
				dp[i+1][nxt][1] %= MOD;
			}
		}
	}

	ll ans = dp[L][0][0] + dp[L][0][1] + MOD - 1;  // 0も答に含まれているため-1する
	ans %= MOD;
	cout << ans << endl;

	return 0;
}
