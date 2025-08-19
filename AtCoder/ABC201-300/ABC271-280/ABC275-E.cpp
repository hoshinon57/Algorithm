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

// ソースコードを2025/8に再解きしたものに置き換えた。
// 先頭の解説は当時のまま。

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

int main(void)
{
	ll i, j;
	ll N, M, K; cin >> N >> M >> K;

	vector<mint> dp(N+1, 0);
	dp[0] = 1;
	mint m_inv = (mint)1/M;
	for(ll k = 1; k <= K; k++)  // k回目
	{
		vector<mint> ndp(N+1, 0);
		for(i = 0; i <= N; i++)  // マスiにいる状態  
		{
			if(i == N)
			{
				ndp[N] += dp[N];  // マスNにいたら何もしない
				continue;
			}
			for(j = 1; j <= M; j++)  // 出目j
			{
				ll nxt = i+j;
				if(i+j > N) nxt = 2*N-i-j;
				//ndp[nxt] += dp[i]/M;
				ndp[nxt] += dp[i] * m_inv;  // 毎回除算は時間がかかる
			}
		}
		swap(dp, ndp);
	}
	cout << dp[N].val() << endl;

	return 0;
}
