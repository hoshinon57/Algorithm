#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;

// ABC275 https://atcoder.jp/contests/abc275

/*
 * 動的計画法(確率DP)で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i回目までルーレットを回したときに、 (i=0～K)
 *   マスjにいる (j=0～N)
 * 確率 (mod 998244353)
 * 
 * 配るDPで考えたときに、ルーレットの目は1～Mで等確率で出るため、
 * マスがj->next_jへ進むとすると、
 *   dp[i+1][next_j] = dp[i][j] / M
 * となる。
 * mod 998244353で求めるため、Mの除算は逆元を求めて積算する。
 * またnext_jは、Nマスから折り返すか否かで計算式が変わる。
 * 
 * 以上を「全K回のルーレット」「ルーレットを回すときのマス」「ルーレットの目」
 * それぞれについて3重ループすればよい。
 * マスNに到達したらゲーム終了のため、dp[*][N]はDP遷移に使ってはいけない点に注意。
 * 
 * 答はdp[*][N]の総和となる。
 */

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
	int N, M, K;
	cin >> N >> M >> K;
	// dp[i][j]:
	//   i回目までルーレットを回したときに、 (i=0～K)
	//   マスjにいる (j=0～N)
	// 確率 (mod 998244353)
	vector<vector<ll>> dp(K+1, vector<ll>(N+1, 0));
	dp[0][0] = 1;  // 初期値 ゲームを始める前はマス0

	int i, j, m;
	ll m_inv = modinv(M, MOD);  // Mで割るため、Mの逆元を求めておく
	// 配るDP
	for(i = 0; i < K; i++)  // i回目のルーレット 配るDPのためi=0～K-1まで
	{
		for(j = 0; j < N; j++)  // マスj  マスNに到達したらゲーム終了のため、マスNからはDP遷移させない
		{
			for(m = 1; m <= M; m++)  // m:ルーレットの目
			{
				int next_j;  // 移動先のマス
				if(j+m <= N)  // 折り返さない場合
				{
					next_j = j + m;
				}
				else  // 折り返す場合
				{
					// j+m-Nマスぶん余るので、
					// その数だけNマス目から戻る
					next_j = N - ((j+m)-N);
				}
				dp[i+1][next_j] += (dp[i][j] * m_inv) % MOD;
				dp[i+1][next_j] %= MOD;
			}
		}
	}

	// ルーレットの1回目～K回目でそれぞれゴールする確率の、総和が答となる
	ll answer = 0;
	for(i = 1; i <= K; i++)
	{
		answer += dp[i][N];  // dp[i][N]:ルーレットのi回目でゴール
		answer %= MOD;
	}
	cout << answer << endl;

	return 0;
}
