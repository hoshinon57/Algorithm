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

// ABC127 https://atcoder.jp/contests/abc127

/*
 * 数え上げ問題。1個目に置くコマを固定して考える。
 * 
 * まず、マス目が3*3で左上に1個目を置くことを考える。このときマス目に対するコストは以下のようになる。
 *   0 1 2
 *   1 2 3
 *   2 3 4
 * この総和をsumとする。
 * 
 * 残りのコマの置き方をC通りとすると、(N*M-1)マスから(K-1)個を選ぶことになり、(N*M-1)C(K-1)通りとなる。
 * 全パターンを見たときに、各マスにコマが置かれる回数は、マスによらず同じになる。
 * ※例えば2*2でK=3のとき、各マスに2回ずつコマが置かれる。
 * これは C*(K-1)/(N*M-1) で求められるので、これにsumをかけたものが
 * 左上のマスに1個目を置いたときのコストの総和となる。
 * 
 * あとは1個目を置くマスをずらした場合のsumを考えればよい。
 * (i,j)から右に行くと、移動先の(i,j)について
 *   sumはi*N増え、(M-j)*N減る。
 *   例：3*3で(i,j)=(0,1)へ移動した場合
 *     1 0 1
 *     2 1 2
 *     3 2 3    左1列は1ずつ増え、右2列は2ずつ増える
 * (i,j)から下に行くと、移動先の(i,j)について
 *   sumはj*M増え、(N-i)*M減る。
 * よって各マスについてO(1)で計算できる。実装では事前計算している。
 * 
 * 2つのマス(xi,yi),(xj,yj)についてi<jという制約があるので、最後に2で割る。
 * 
 * [ACまでの思考の流れ]
 * ・数え上げは考え方を変えて、
 *   1つ目のコマの置き方について全探索し、それぞれについてコストを求める
 *   が良さそう。
 * ・2つのマス(xi,yi),(xj,yj)についてi<jという制約があるが、
 *   これは制約をいったん無視して全組み合わせを求め、最後に2で割ればよい。
 *   この場合、i=jのときコストが0という前提。コストが非0なら減算しておく。
 */

const ll MOD = 1e9+7;
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
	ll i, j;
	ll N, M, K; cin >> N >> M >> K;
	nCk_init();
	vector<vector<ll>> sum(N, vector<ll>(M, 0));  // sum[N][M]
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++) sum[0][0] += i+j;
	}
	sum[0][0] %= MOD;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++)
		{
			if(i==0 && j==0) continue;
			if(j != 0)  // 左からもらう
			{
				sum[i][j] = sum[i][j-1] + j*N  - (M-j)*N;
			}
			else  // 上からもらう
			{
				sum[i][j] = sum[i-1][j] + i*M  - (N-i)*M;
			}
			sum[i][j] %= MOD;
		}
	}

	ll ans = 0;
	ll C = nCk(N*M-1, K-1);  // 残りのマスの置き方
	ll NM_inv = modinv(N*M-1, MOD);
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++)
		{
			ans += C * (K-1) % MOD * NM_inv % MOD * sum[i][j] % MOD;
			ans %= MOD;
		}
	}
	ans *= modinv(2, MOD);
	ans %= MOD;
	cout << ans << endl;

	return 0;
}
