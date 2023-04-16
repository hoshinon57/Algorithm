#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC298 https://atcoder.jp/contests/abc298

/*
 * コンテスト中に解けず、解説を見た。
 * 確率DPで解く。
 * 以下、1-indexedで記述する。
 * 
 * dp[101][101][2]を以下のように定義する。
 *   [高橋マス][青木マス][次が 0:高橋ターン 1:青木ターン] のときに
 *   高橋くんが勝つ確率
 * 
 * DP初期値は以下となる。
 *   dp[N][*][*]=1 (高橋くんがゴールしているので)
 *   dp[*][N][*]=0 (青木くんがゴールしているので)
 * 
 * DP遷移は以下となる。ゴール側から計算していく。
 * (1)dp[i][j][0] を求めたい場合 （高橋くんのターン)
 *   高橋くんは i+1, i+2, ..., i+P まで等確率で移動するので(ただしNとのminを取る)、
 *   各k(1～P)について dp[i+k][j][1]/P を加算する。 (次は相手のターンなので、[1]を使う)
 * (2)dp[i][j][1] を求めたい場合
 *   (1)と同様に考えて、
 *   各k(1～Q)について dp[i][j+k][0]/P を加算する。
 * 
 * dp[A][B][0] が答となる。
 * 
 * P,Qで割る場合の逆元を事前計算しておくことで、計算量はO(N^2*(P+Q)).
 */

ll dp[101][101][2];

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
	// 1-indexed
	ll i, j, k;
	ll N, A, B, P, Q;
	cin >> N >> A >> B >> P >> Q;

	// dp[N][*][*]は高橋くんの勝ち
	for(i = 1; i < N; i++)
	{
		dp[N][i][0] = dp[N][i][1] = 1;
	}
	// dp[*][N][*]は青木くんの勝ち
	for(i = 1; i < N; i++)
	{
		dp[i][N][0] = dp[i][N][1] = 0;
	}

	// 後ろから順にdp[]を求めていく
	ll P_inv = modinv(P, MOD);  // P, Qで割る
	ll Q_inv = modinv(Q, MOD);
	for(i = N-1; i >= 1 ; i--)
	{
		for(j = N-1; j >= 1 ; j--)
		{
			ll sum = 0;
			// 高橋くんがサイコロを振る
			for(k = i+1; k <= i+P; k++)
			{
				sum += dp[min(k,N)][j][1] * P_inv;  // 次は青木くんのターンなので[1]
				sum %= MOD;
			}
			dp[i][j][0] = sum;

			// 青木くんがサイコロを振る
			sum = 0;
			for(k = j+1; k <= j+Q; k++)
			{
				sum += dp[i][min(k,N)][0] * Q_inv;  // 次は高橋くんのターンなので[0]
				sum %= MOD;
			}
			dp[i][j][1] = sum;
		}
	}

	cout << dp[A][B][0] << endl;

	return 0;
}
