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

// 再解き時にソースコード差し替えた

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
**/

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

const ll MOD = 998244353;
const ll NCK_SIZE = 2000001;  // N<=2*10^5 など、問題文の制約に合わせる
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
	nCk_init();
	ll y, x;
	ll H, W, K; cin >> H >> W >> K;
	mint ans = 0;
	mint tot = nCk(H*W, K);  // Kマス選ぶ全パターン数
	auto cal = [&](ll yy, ll xx) -> mint
	{
		return nCk(yy*xx, K);
	};
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)  // マス(y,x)について計算
		{
			mint t = tot;
			// (x,y)の上側のみ、下側のみ、左側のみ、右側のみ
			t -= cal(y, W);
			t -= cal(H-y-1, W);
			t -= cal(H, x);
			t -= cal(H, W-x-1);
			// (x,y)の左上側のみ、右上側、左下側、右下側
			t += cal(y, x);
			t += cal(y, W-x-1);
			t += cal(H-y-1, x);
			t += cal(H-y-1, W-x-1);
			ans += t;
		}
	}
	ans /= tot;
	cout << ans.val() << endl;

	return 0;
}
