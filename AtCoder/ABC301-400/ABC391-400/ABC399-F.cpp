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

// ABC399 https://atcoder.jp/contests/abc399

/*
 * ぼちぼちと解説をチラ見して解いた。
 * 典型としては「区間の和は累積和で考える」か。
 * 
 * 数列Aの累積和をSとして表す。
 * またl<rとして区間を[l,r)の半開区間で見ると、元の式は
 *   Σ[0<=l<r<=N](Sr-Sl)^K
 * と表せる。ゴリゴリと式変形していくと、二項定理も用いて、
 *   Σ[0<=l<r<=N](Σ[i=0～K]C(K,i)*Sr^i*(-Sl)^(K-i) )
 * rを固定するイメージで、
 *   Σ[0<r<=N](Σ[i=0～K]C(K,i)*Sr^i*{(-Sl)^(K-i)+(-S(l-1))^(K-i)+(-S(l-2))^(K-i)+...+(-(S0)^(K-i)))} )
 * = Σ[0<r<=N](Σ[i=0～K]C(K,i)*Sr^i*{((-S0)^(K-i)) + ((-S1)^(K-i)) + ... + ((-S(r-1))^(K-i)) }
 * となる。
 * {}の部分は、rを昇順に見ていって ^(K-i) のパターンだけ持って更新していけばよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・式変形を頑張る。
 * ・区間の和は累積和で考える。
 * ・二重シグマにてrを固定(rを伸ばしていく)方針も考えてみる。
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
	ll i;
	ll N, K; cin >> N >> K;
	vector<mint> a(N);
	for(i = 0; i < N; i++)
	{
		ll tmp; cin >> tmp;
		a[i] = tmp;
	}
	vector<mint> as(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {as[kk+1] = as[kk] + a[kk];}

	mint ans = 0;
	// S0～S(r-1)それぞれのi乗の和
	// 初期値はS0=0なので、0^0のみ1, 他の0^iは0.
	vector<mint> ssum(K+1, 0);
	ssum[0] = 1;
	for(ll r = 1; r <= N; r++)
	{
		for(i = 0; i <= K; i++)
		{
			mint t = nCk(K, i);
			t *= as[r].pow(i);  // Sr^i
			t *= ssum[K-i];  // (-Sx)^(K-i) のS0～S(r-1)までについての和
			ans += t;
		}
		for(i = 0; i <= K; i++)
		{
			ssum[i] += (mint)(-as[r]).pow(i);
		}
	}
	cout << ans.val() << endl;

	return 0;
}
