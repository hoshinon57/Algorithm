#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC313 https://atcoder.jp/contests/abc313

/*
 * コンテスト中に解けず、解説を見た。
 * 動的計画法（っぽく）解く。
 * 
 * まず、操作が無限に続くのはどういうケースかを試行錯誤すると、
 * 「2以上の数値が連続する」ケースになりそう。
 * よって最初にそれを判別してしまう。
 * 
 * 以下、0-indexedで記載する。
 * dp[i]を、i文字目が消えるまでの回数と定義する。
 * S="3131"を例とすると、1文字目の1の部分は
 *   操作ごとに 1 -> 111 -> 11111 と(3-1)文字ずつ増えていく。
 *   増える回数は、2文字目の3が消えるまでの回数と一致し、これはdp[2]となる。
 * よって1文字目の1が消えるまでの回数dp[1]は、
 *   (1)次の文字"3"が消え終わるまで：dp[2]
 *   (2)1が増える個数：(3-1)*dp[2]
 *   (3)1文字目の"1"自身が消えるのが1回：1
 * より、dp[1] = (1)+(2)+(3) となる。
 * ※"11111"のように1が連続する場合、1操作あたり1文字ずつ減っていく。
 * これを末尾から順に計算していけばよい。
 * 
 * 答は「長さが1になるまでの操作回数」であり、
 * dp[0]だと「長さが0になるまでの操作回数」となってしまうので、
 * dp[0]-1 が答。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	for(i = 0; i < N-1; i++)
	{
		if(s[i] != '1' && s[i+1] != '1')
		{
			cout << -1 << endl;
			return 0;
		}
	}

	vector<ll> dp(N+1);  // dp[i]:i文字目が消えるまでの回数
	dp[N-1] = 1;  // 末尾は1回で消える
	for(i = N-2; i >= 0; i--)
	{
		int c = (int)(s[i+1]-'0');
		dp[i] = 1 + (c-1)*dp[i+1] + dp[i+1];
		dp[i] %= MOD;
	}
	cout << (dp[0]+MOD-1)%MOD << endl;
	
	return 0;
}
