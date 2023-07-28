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

// ABC132 https://atcoder.jp/contests/abc132

/*
 * 自力で解けず、解説を見た。
 * 組み合わせで解く。
 * 
 * 「i回操作が必要」は「青いボールがiグループに分かれている」と読み替えることができる。
 * よって 1<=i<=K の各iについて、いわゆる最小単位である
 *   青赤青…赤青  (青がi回)
 * と1個ずつ交互に並んだ状態を考える。
 * ここに残りの青と赤を入れていく。
 * 
 * 上記の最小単位では青がi個、赤がi-1個であるので、
 * 青ボールの残数：K-i
 * 赤ボールの残数：(N-K)-(i-1)
 * 青のグループ数：i
 * 赤のグループ数：i+1 (最小単位には含まれない両端にも2個入れられるため、i-1+2)
 * となる。
 * 
 * 「x個を、i個のグループに振り分ける」ときの場合の数は (x+i-1)C(x) となるので、
 * これの青赤それぞれの積が、i回操作のときの答。
 */

const ll MOD = 1000000000+7;
const ll NCK_SIZE = 2001;  // N<=2*10^5 など、問題文の制約に合わせる
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
	ll i;
	ll N, K; cin >> N >> K;
	nCk_init();

	for(i = 1; i <= K; i++)  // i回操作
	{
		// まず、1個ずつ 青赤青…赤青 と並べたものを考える。このとき青がi個、赤がi-1個となる。
		ll b_zan = K-i;  // 青ボール残り
		ll r_zan = (N-K) - (i-1);  // 赤ボール残り
		ll b_gr = i;  // 青のグループ数
		ll r_gr = i+1;  // 赤のグループ数 (両端にも置けるので、i-1+2)
		ll b = nCk(b_zan+b_gr-1, b_zan);
		ll r = nCk(r_zan+r_gr-1, r_zan);
		cout << (b*r)%MOD << endl;
	}

	return 0;
}
