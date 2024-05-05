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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 各種解説を元に包除原理で解いてみた。
 *   https://zenn.dev/kiwamachan/articles/5ee24fadfce6a8
 *   https://drken1215.hatenablog.com/entry/2019/01/16/000100
 *   https://blog.hamayanhamayan.com/entry/2019/01/12/160257
 * 
 * dp[i][m]を以下のように定義する。
 *   壁iに到達したときに、  (i=1～N+1 後述)
 *   それまでに通った壁の数が m=0:偶数, 1:奇数 (壁iを含む)
 *   ときの、ルート総数
 * 頂点数を+2し、スタートとゴールも入れる。
 * 座標でソートするため、スタートは頂点0, ゴールはN-1となる(Nを+2した後の値)。
 * また dp[0][0]=1, dp[0][1]=0 としておく。(定義の問題なので、逆でもよい)
 * 
 * すると包除原理より、
 *   m=1のルート数は答に加算
 *   m=0のルート数は答に減算
 * となるので、 dp[N-1][1]-dp[N-1][0] が答となる。
 * 
 * EDPC-Y.cppと同じく、壁マスは座標でソートしておく。
 * するとdp[i][*]は、もらうDPにて
 *   dp[i][0] = Σdp[j][1] * (壁j->壁iへのルート)   ※j<i つまりiより手前にある壁j
 *   dp[i][1] = Σdp[j][0] * (壁j->壁iへのルート)   ※j<i つまりiより手前にある壁j
 * と計算できる。
 */

const ll MOD = 1e9+7;
const ll NCK_SIZE = 200005;  // N<=2*10^5 など、問題文の制約に合わせる
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

int main(void)
{
	// 0-indexed
	ll i, j, m;
	ll H, W, N; cin >> H >> W >> N;
	vector<pair<ll,ll>> wa;  // 壁の位置{r,c}
	for(i = 0; i < N; i++)
	{
		ll r, c; cin >> r >> c;
		r--; c--;
		wa.push_back({r, c});
	}
	N += 2;  // スタートとゴールを頂点に追加
	wa.push_back({0, 0});
	wa.push_back({H-1, W-1});
	sort(wa.begin(), wa.end());
	nCk_init();

	// (sy,sx)->(gy,gx)への通り数
	auto cal = [&](ll sy, ll sx, ll gy, ll gx) -> ll
	{
		ll dy = gy-sy;
		ll dx = gx-sx;
		return nCk(dy+dx, dx);
	};

	vector<vector<ll>> dp(N, vector<ll>(2, 0));  // dp[N][2]
	// DP初期値を以下のように定義しておくと(スタート地点は通った壁マス数としてカウントしない)、
	// dp[N-1][1]-dp[N-1][0]が解となる
	dp[0][0] = 1;
	dp[0][1] = 0;
	// もらうDP
	for(i = 1; i <= N-1; i++)
	{
		for(m = 0; m < 2; m++)  // dp[i][m]
		{
			for(j = 0; j < i; j++)
			{
				// dp[i][m] = Σ(dp[j][1-m] * (壁j->iへのルート数))
				ll r = cal(wa[j].first, wa[j].second, wa[i].first, wa[i].second);
				dp[i][m] += dp[j][1-m] * r;
				dp[i][m] %= MOD;
			}
		}
	}
	ll ans = dp[N-1][1] - dp[N-1][0] + MOD;
	ans %= MOD;
	cout << ans << endl;

	return 0;
}
