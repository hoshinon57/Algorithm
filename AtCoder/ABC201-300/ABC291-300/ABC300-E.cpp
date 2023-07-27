#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC300 https://atcoder.jp/contests/abc300

/*
 * 優先度付きキューを使い、確率DP(配るDP)で解く。
 * 
 * まず、サイコロの出目は2～6の各1/5の確率と考えてよい。
 * 出目1は振りなおしと同等であるため。
 * 
 * dp[i]を「数値iスタートで、最終的にNになる確率」として定義する。
 * 初期値としてdp[N]=1.
 * 配るDPについて、あるdp[i]から
 *   dp[i/2] += dp[i]/5
 *   dp[i/3] += dp[i]/5
 *   ...
 *   dp[i/6] += dp[i]/5
 * と遷移させればよい。
 * (数i/2からiへは、1/5で遷移するという考え方)
 * iが大きい側から遷移計算したいので、優先度付きキューを用いる。
 * ⇒iについて遷移計算しつつ、i/2, i/3, ..., i/6をキューに積む。
 * 
 * Nは最大10^18になるため、配列ではなくmapで管理する。
 * 
 * 例えばNについて、N/4は
 *   N->N/4 と N->N/2->N/4
 * の2パターンの遷移があるため、2回キューに積まれることになる。
 * 遷移計算を2回行うと値がおかしくなるので、1回に抑える必要がある。
 * これはsetで管理する。(実装ではdp_flag)
 * 
 * 以上をdp[N]から開始し、dp[1]が答。
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

int main(void)
{
	const ll div_5 = modinv(5, MOD);  // 5で割る
	ll i;
	ll N;
	cin >> N;

	priority_queue<ll> que;  // 大きい方が優先度が高い
	// dp[i]:数値iスタートで、最終的にNになる確率
	map<ll, ll> dp;
	set<ll> dp_flag;  // iがあればdp[i]から配る処理は実施済み
	dp[N] = 1;
	que.push(N);
	while(!que.empty())
	{
		ll v = que.top();
		que.pop();
		if(dp_flag.count(v) > 0) continue;  // dp[v]から配る処理は実施済み
		dp_flag.insert(v);

		// v/2, v/3, ..., v/6についてそれぞれ配る
		for(i = 2; i <= 6; i++)
		{
			if(v%i != 0) continue;
			dp[v/i] += dp[v]*div_5;
			dp[v/i] %= MOD;
			que.push(v/i);
		}
	}
	cout << dp[1] << endl;
	
	return 0;
}
