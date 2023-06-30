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

// ABC303 https://atcoder.jp/contests/abc303

/*
 * 動的計画法で解く。
 * 以下、Nを文字列Sの長さとする。
 * 
 * dp[i][j]を以下のように定義する。
 *   i文字目まで入力したときに、  (i=0～N)
 *   CapsLockの状態が、j=0:OFF, j=1:ON
 *   であるときの最短の時間
 * 
 * DP遷移計算は、
 * ・Aを押すとき
 *   CapsLock=OFFの状態でShift+a
 *   CapsLock=ONの状態でa
 * ・aを押すとき
 *   CapsLock=OFFの状態でa
 *   CapsLock=ONの状態でShift+a
 * としてまず計算し、
 * 次にCapsLockを押してdp[i][0]とdp[i][1]間の遷移を考えるようにすると、簡易に実装できる。
 * 
 * dp[N][0]とdp[N][1]のうち小さい方が答。
 */
int main(void)
{
	int i;
	ll x, y, z; cin >> x >> y >> z;
	string s; cin >> s;

	int s_size = (int)s.size();
	vector<vector<ll>> dp(s_size+1, vector<ll>(2, INF64));  // dp[s_size+1][2]
	dp[0][0] = 0;
	dp[0][1] = z;  // 初期状態でまずCapsLockを押す
	for(i = 1; i <= s_size; i++)
	{
		if(s[i-1] == 'A')  // i文字目
		{
			dp[i][0] = dp[i-1][0]+y;  // CapsLock=OFFの状態でShift+a
			dp[i][1] = dp[i-1][1]+x;  // CapsLock=ONの状態でa
		}
		else
		{
			dp[i][0] = dp[i-1][0]+x;
			dp[i][1] = dp[i-1][1]+y;
		}

		// CapsLock ON/OFF間の遷移
		dp[i][0] = min(dp[i][0], dp[i][1]+z);
		dp[i][1] = min(dp[i][1], dp[i][0]+z);
	}
	cout << min(dp[s_size][0], dp[s_size][1]) << endl;

	return 0;
}
