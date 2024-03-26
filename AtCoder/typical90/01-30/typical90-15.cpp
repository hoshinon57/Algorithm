#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool
// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

// 競プロ典型90問:15 https://atcoder.jp/contests/typical90/tasks/typical90_o

/*
 * 自力で解けず、解説を見た。
 * 調和級数の考え方で解く。
 *   https://twitter.com/e869120/status/1382827276673306624
 * 
 * 基本は公式解説の通り。
 * 例えばN=10,k=3のとき、選べるボールは最大で1,4,7,10番目の4個となる。これはceil(N/k)となる。
 * つまり、kが大きいと選べるボールの種類は少ないということが分かる。
 * 各kについて、選べるボールの数ごとに計算していくとすると、ループ回数は
 *   ceil(N/1) + ceil(N/2) + ... + ceil(N/N)
 * となり、調和級数によりO(NlogN)となる。
 * よって、kとその際の選べるボールの数で全探索しても間に合うということ。
 * 
 * では差kと選ぶボール数xを決めたときに、何パターンあるかを考える。
 * yを選んだボール間に挟むボールの数とする。k=3ならボールごとに2個入れるイメージ。
 * これは y=(x-1)*(k-1) となる。
 * またzをx,y以外の残りのボール数とすると、 z=N-x-y.
 * パターン数はCombinationの (x+z)C(x) 通りとなる。
 * これを各xについて加算していけばよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・いやーこれは難しい…
 * ・各kについて、ボールを何個まで選べるかを考えてみる。
 *   ceil(N/k)が最大なので、すると各kごとに加算すると調和級数が見えてくる。
 * ・差kと選ぶボール数xが決まったときに、そこから (x+z)C(x) をどうやったら見えるか。
 *   「スキマとして使うボール」など、必要なボールを洗い出していく感じかなあ。
 */

const ll MOD = 1e9+7;
const ll NCK_SIZE = 100001;  // N<=2*10^5 など、問題文の制約に合わせる
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
	ll N; cin >> N;
	for(ll k = 1; k <= N; k++)
	{
		ll ans = 0;
		ll x, y, z;
		for(x = 1; x <= ceil_div(N,k); x++)  // 選ぶボールの数x
		{
			y = (x-1)*(k-1);  // 差がk以上となるよう、ボール間のスキマとして使うボールの数y
			z = N-x-y;  // 余りのボール
			ans += nCk(x+z, x);
			ans %= MOD;
		}
		cout << ans << endl;
	}	

	return 0;
}
