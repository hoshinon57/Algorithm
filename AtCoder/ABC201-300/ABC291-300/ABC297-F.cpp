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

// ABC297 https://atcoder.jp/contests/abc297

/*
 * 自力で解けず、解説を見た。
 * 主客転倒、期待値の線形性などを用いて解く。
 *   https://atcoder.jp/contests/abc297/editorial/6157
 *   https://qiita.com/viral_8/items/21ed8b6835d7f6c44bcf
 *   https://scrapbox.io/procon-kirokuyou/ABC297_F_-_Minimum_Bounding_Box_2_(500)
 *   https://scrapbox.io/kani/ABC297_-_F_-_Minimum_Bounding_Box_2
 * 
 * 基本は公式解説の通り。
 * 方針は、全ての塗り方に対するスコアの総和を、塗り方の場合の数で割る。
 * ここで主客転倒を用いて、マス(x,y)について以下を求め、x,yについて全探索する。
 *   マス(x,y)が長方形に含まれるような塗り方の個数
 * 
 * これは、選ぶマスが
 *   1:全てxより左側にある
 *   2:全てxより右側にある
 *   3:全てyより上側にある
 *   4;全てyより下側にある
 * とき、(x,y)は長方形に含まれないので、全ての塗り方から引けば求められる。
 * これは包除を用いて解ける。(1～4を1つ選ぶ場合は加算し、2つ選ぶ場合は減算する)
 * 
 * 全ての(x,y)について計算したら、その総和を全ての塗り方の個数で割れば答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・期待値は「全ての事象に対する答の総和 ÷ 事象の場合の数」で求められる。
 *   本問では、全ての塗り方に対するスコアの総和を、塗り方の場合の数で割るという意味になる。
 */

const ll MOD = 998244353;
const ll NCK_SIZE = 1000005;  // N<=2*10^5 など、問題文の制約に合わせる
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
	nCk_init();
	ll H, W, K; cin >> H >> W >> K;
	ll nck_HW_K = nCk(H*W, K);  // 塗り方の場合の数  各所で使うので事前計算
	ll ans = 0;
	for(ll x = 0; x < W; x++)
	{
		for(ll y = 0; y < H; y++)  // マス(x,y)について
		{
			// m1～m4:(x,y)の左側のみ、右側のみ、上側のみ、下側のみ
			ll m1 = nCk(x*H, K);
			ll m2 = nCk((W-(x+1))*H, K);
			ll m3 = nCk(W*y, K);
			ll m4 = nCk(W*(H-(y+1)), K);
			// m5～m8:(x,y)の左上側のみ、右上側、左下側、右下側
			ll m5 = nCk(x*y, K);
			ll m6 = nCk((W-(x+1))*y, K);
			ll m7 = nCk(x*(H-(y+1)), K);
			ll m8 = nCk((W-(x+1))*(H-(y+1)), K);
			ll tmp = ((m1+m2+m3+m4-m5-m6-m7-m8) + (MOD*4)) % MOD;  // この値を全体から除外する
			tmp = (nck_HW_K - tmp + MOD) % MOD;
			ans = (ans+tmp) % MOD;
		}
	}
	ans = ans * modinv(nck_HW_K, MOD) % MOD;
	cout << ans << endl;

	return 0;
}
