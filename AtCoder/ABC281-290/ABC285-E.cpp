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

// ABC285 https://atcoder.jp/contests/abc285

/*
 * 自力で解けず、解説を見た。
 * 動的計画法で解く。
 * 以下、0-indexedで記載する。
 * 
 * 1週間に1日以上は休日が必要のため、0日目を休日とする。
 * dp[i]を以下のように定義する。
 *   i日目を休みとしたときの、その日までの生産量の最大値  (i=0～N)
 * 
 * また、work[i]を
 *   平日がi日連続したときの、その間の生産量の総和
 * と定義する。
 * work[0]=0として、work[i]はwork[i-1]から求められる。
 * work[i-1]が分かっているときに、追加の平日1日を、(i-1)日の平日の中央に追加するイメージで、
 *   work[i] = work[i-1] + a[(i+1)/2]; となる。
 * 
 * するとdp[i]は、休日がj日ぶりのときに、全てのjに対して
 *   max(dp[i-j] + work[j-1])
 * として求められる。
 * 
 * 0日目およびN日目が休日となり、1週間の生産量を求めたいため、
 * dp[N]まで計算すれば、dp[N]が答となる。
 */
int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}  // 1-indexed
	vector<ll> work(N);
	for(i = 1; i < N; i++)
	{
		work[i] = work[i-1] + a[(i+1)/2];
	}

	vector<ll> dp(N+1);  // dp[N]まで計算する
	for(i = 1; i <= N; i++)  // i日目を休みとする
	{
		for(j = 1; j <= N; j++)  // 休日がj日ぶり
		{
			if(i-j < 0) continue;
			dp[i] = max(dp[i], dp[i-j] + work[j-1]);
		}
	}

	cout << dp[N] << endl;

	return 0;
}
