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
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC169 https://atcoder.jp/contests/abc169

/*
 * DP, 主客転倒で解く。
 * 
 * 例えばN=7で、3つの要素で和Sを満たすものがあるとする、
 * これがどれぐらい答に寄与するか？を考える。
 * 残り7-3=4要素は、部分集合Tにあるケースと無いケース、どちらもカウントされることから
 * 2^4=16 が答に加算される。
 * 
 * つまりA1,A2,...,An についてナップザックDPをやったときに、dp[i]が
 *   Aiを選ばない：dp[i-1]と同じ
 *   Aiを選ぶ：dp[i-1]/2 を加算
 * となるイメージ。
 * 
 * dp[i][j]を以下のように定義する。
 *   i個目まで見て、 (i=1～N)
 *   和がjとなるような組み合わせにて、  (j=0～S)
 *   そのような部分集合Tの個数
 * 
 * DP遷移は前述の通り。
 * DP初期値はdp[0][0]=2^N.
 * dp[N][S]が答。
 * 
 * [ACまでの思考の流れ]
 * ・数え上げなので主客転倒を考えてみる。
 * ・例えばN=5で2要素から和Sを作れる場合、そのような部分集合Tは何パターン作れるか。
 *   ⇒Aから選ぶごとに、Tの数は半分になっていきそう。
 */

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	int i, j;
	int N, S; cin >> N >> S;
	vector<int> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<vector<ll>> dp(N+1, vector<ll>(S+1, 0));  // dp[N+1][S+1]
	dp[0][0] = modpow(2, N, MOD);

	ll div2 = modinv(2, MOD);
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= S; j++)
		{
			dp[i][j] = dp[i-1][j];
			if(j-a[i] >= 0)
			{
				ll tmp = dp[i-1][j-a[i]] * div2 % MOD;
				dp[i][j] += tmp;
				dp[i][j] %= MOD;
			}
		}
	}
	cout << dp[N][S] << endl;

	return 0;
}
