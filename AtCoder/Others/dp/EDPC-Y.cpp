#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cassert>
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
 * 以下と同じ方針で解いた。
 *   https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106
 *   https://qiita.com/Series_205/items/05bd22a4e78527cb29dc
 *   https://blog.bobuhiro11.net/2020/04-26-edpc.html
 * ※後述のdp[i]を求めるときに、i<jとするか j<iとするかの違いがある。
 *   自分は前者で実装した。
 * 
 * dp[i]を以下のように定義する。
 *   i番目の壁について考えたときに、
 *   スタートから他の壁には行かずに、i番目の壁マスに到達する経路数
 *   ※壁iに「最初にぶつかる」という考え方
 * 
 * 答ansは以下のように求まる。
 *   ans = (壁の存在を無視して、スタートからゴールまでの経路数)
 *         - (スタートからゴールまで、壁マスを通る経路数)
 * dp[i] * (壁i->ゴールまでの経路数) を各iについてansから減算すれば良い。
 * (壁i->ゴールについては、他の壁の存在を無視することができる。)
 * 
 * dp[]の定義 "そこに到達するまでに他の壁には行かずに" より、
 * 壁iを通って壁jに行く経路はdp[j]から減らしておく必要がある。
 * よって壁は座標順にソートしてからアクセスする。
 * 
 * あとは実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・求めるものの余事象で考えてみる。
 *   本問では「壁を無視したときの総数」から「壁を通るときの総数」を引いた。
 * ・「最初/最後に***したもの」をdp[]に持たせてみる
 * ・DPにて要素を使う順番が任意であるときに、実は最適な順序が一意に決まるケースがある、という考え方。
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
	ll i, j;
	ll H, W, N; cin >> H >> W >> N;
	vector<pair<ll,ll>> wa(N);  // 壁の位置{r,c}
	for(i = 0; i < N; i++)
	{
		cin >> wa[i].first >> wa[i].second;
		wa[i].first--;
		wa[i].second--;
	}
	sort(wa.begin(), wa.end());
	nCk_init();

	// (sy,sx)->(gy,gx)への通り数
	auto cal = [&](ll sy, ll sx, ll gy, ll gx) -> ll
	{
		ll dy = gy-sy;
		ll dx = gx-sx;
		return nCk(dy+dx, dx);
	};
	assert(cal(0, 0, 1, 1) == 2);
	assert(cal(0, 0, 4, 4) == 70);
	assert(cal(10, 10, 12, 13) == 10);
	assert(cal(0, 0, 0, 4) == 1);
	assert(cal(2, 2, 10, 2) == 1);
	assert(cal(1, 3, 3, 1) == 0);
	assert(cal(1, 3, 0, 4) == 0);

	vector<ll> dp(N);
	ll ans = nCk(H+W-2, H-1);  // (H-1+W-1)C(H-1)  壁の存在を無視した場合の、経路数
	for(i = 0; i < N; i++)
	{
		auto [y,x] = wa[i];
		// (スタート->壁i) * (壁i->ゴール) への経路数だけansから引く
		dp[i] += cal(0, 0, y, x);
		ll toG = cal(y, x, H-1, W-1);
		ans = ans + MOD - (dp[i] * toG) % MOD;
		ans %= MOD;

		// dp[]の定義 "そこに到達するまでに他の壁には行かずに" より、
		// 壁iを通って壁jに行く経路はdp[j]から減らしておく必要がある
		for(j = i+1; j < N; j++)
		{
			ll z = cal(y, x, wa[j].first, wa[j].second);  // 壁i->壁j
			dp[j] = dp[j] + MOD - (dp[i] * z % MOD);
			dp[j] %= MOD;
		}
	}
	cout << ans << endl;

	return 0;
}
