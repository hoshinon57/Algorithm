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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC406 https://atcoder.jp/contests/abc406

/*
 * kyopro_friends氏の解説を元に実装した内容。
 *   https://atcoder.jp/contests/abc406/editorial/13078
 * 
 * とはいえ色々と消化不良だった…
 * 0除算の部分に苦戦したり、どうもll型でもオーバーフローが起きてそうだったりと、
 * WAが全然取れなかった。
**/

const ll MOD = 998244353;
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

// 2^b<=x となる最大のbを返す
// 例)f(1)=0, f(3)=1, f(4)=2, f(5)=2
// 2^b<=0 はありえないのでf(0)は適当に0を返す
ll sub(ll x)
{
	ll b;
	for(b = 0; b < 63; b++)
	{
		if((1LL<<(b+1)) <= x) ;  // 1つ増やしても大丈夫なら増やす
		else break;
	}
	return b;
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

using pll = pair<ll,mint>;  // {個数,総和}
// n以下でpopc=k
pll calc(ll n, ll k)
{
	if(n == 0)  // ベースケース
	{
		if(k == 0) return {1, 0};
		else return {0, 0};
	}
	if(k == 0) return {1, 0};

	ll b = sub(n);
	ll p = 1LL<<b;  // 2^b
	pll ans = {0, 0};

	// 2^b未満の範囲で、popc=kの総和を求める
	// パターン数：C(b,k)
	// パターンごとbitがk個、ならしで/b, 値は(2^b-1)
	if(b == 0)
	{
		if(k == 0) ans = {1, 0};
		else ans = {0, 0};
	}
	else
	{
		ans.first = nCk(b, k);
		ans.second = mint(ans.first) * k / b * (p-1);
	}
	// こっちでも良い(公式解説の式)
	// ans.second = p-1;
	// ans.second *= nCk(b-1, k-1);

	// 2^b以上の部分
	auto t = calc(n-p, k-1);
	ans.first += t.first;
	ans.second += t.second + (mint)t.first * p;

	return ans;
}

void solve(void)
{
	ll N, K; cin >> N >> K;
	auto ans = calc(N, K);
	cout << ans.second.val() << endl;
}

int main(void)
{
	nCk_init();
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}
	return 0;
}
