#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC156 https://atcoder.jp/contests/abc156

/*
 * 数え上げ的な問題。
 * 
 * 「人の数の組み合わせ」なので、人ごとの区別はしないのがポイント。
 * どこから考えたものか、となったときに、まず適当にn=5, k=2で考えてみると、
 *   (1)0人の部屋が2つ、1人以上の部屋が3つ
 *   (2)0人の部屋が1つ、1人以上の部屋が4つ
 *   (3)0人の部屋が0つ、1人　　の部屋が5つ
 * という組み合わせが可能となる。
 * ここで(2),(3)は「人の数の組み合わせ」の観点で無駄移動をするイメージ。
 * 
 * (1)について組み合わせを数えると、
 *   0人の部屋の作り方：5C2
 *   1人以上の部屋の作り方：
 *     元々1人だけいた部屋が3つあり、そこに2人が入ってくる形になる。
 *     重複組み合わせの「仕切り」の考え方で、(3-1+2)C2
 * の積となる。
 * k=iについて一般化すると、前者はnCi, 後者は(n-1)Ciとなる。
 * 
 * n回以上の移動は意味が無い（無駄移動になる）ことを考えると、kはn-1で上限クリップしてよく、
 * k=1～n-1についての総和が答となる。
 * 実装ではmov[i]とした。
 * 
 * [ACまでの思考の流れ]
 * ・k回移動できるときに、どんな部屋の状態を作れるかを考える。
 *   まずはkが小さいケースから。
 * ・部屋の状態(0人の部屋や1人以上の部屋の数)をイメージできたら、それは何通りあるか計算式を立てる。
 */

const ll NCK_SIZE = 200001;  // N<=2*10^5 など、問題文の制約に合わせる
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

	int i;
	ll n, k; cin >> n >> k;
	k = min(k, n-1);  // 移動は最大n-1で十分
	vector<ll> mov(n);  // mov[i]:無駄な移動なしでi回移動したときの、組みあわせ
	for(i = 0; i <= k; i++)  // i:移動回数
	{
		ll a, b;
		a = nCk(n, i);  // 0人の部屋の作り方
		b = nCk(n-1, i);  // 1人以上の部屋の作り方
		mov[i] = (a * b) % MOD;
	}

	ll ans = 0;
	for(auto &e : mov) ans = (ans + e) % MOD;
	cout << ans << endl;

	return 0;
}
