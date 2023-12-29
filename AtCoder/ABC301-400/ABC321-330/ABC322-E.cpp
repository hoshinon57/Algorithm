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

// ABC322 https://atcoder.jp/contests/abc322

/*
 * 動的計画法で解く。
 * コンテスト後の整理時、参考にしたコード：
 *   https://atcoder.jp/contests/abc322/submissions/46081641
 * 
 * 制約より、K個のパラメータの取りうる組み合わせは (P+1)^K 通りであり、最大で7776通りほど。
 * (パラメータは上限Pでクリップする)
 * dp[i][j]を
 *   i個目の開発案まで見て、 (i=1～N)
 *   K個のパラメータが (p1,p2,...,pK) のときの、 (p=0～(P+1)^K-1)
 *   コストの最小値
 * のように定義すると、dp[i][j]からの遷移はO(K)で処理できるため、全体で O(NK(P+1)^K) となり間に合う。
 * 
 * パラメータの集合は、 p1*K^0+p2*K^1+... のようにP+1進数のK桁とみなした。
 */

int main(void)
{
	int i, j, k;
	int N, K, P; cin >> N >> K >> P;
	vector<ll> c(N);
	vector<vector<int>> a(N, vector<int>(K, 0));  // a[N][K]
	for(i = 0; i < N; i++)
	{
		cin >> c[i];
		for(j = 0; j < K; j++) cin >> a[i][j];
	}
	vector<int> POW(K+1);  // POW[i]:(P+1)^i
	POW[0] = 1;
	for(i = 0; i < K; i++) POW[i+1] = POW[i] * (P+1);
	vector<vector<ll>> dp(N+1, vector<ll>(POW[K], INF64));  // dp[N+1][POW[K]]
	dp[0][0] = 0;
	for(i = 0; i < N; i++)  // i->i+1へ配るDP
	{
		for(j = 0; j < POW[K]; j++)
		{
			chmin(dp[i+1][j], dp[i][j]);  // 開発案を不採用
			// 開発案を採用
			int aft = 0;
			for(k = 0; k < K; k++)
			{
				int tmp = j/POW[k]%(P+1);
				tmp = min(tmp+a[i][k], P);
				aft += tmp*POW[k];
			}
			chmin(dp[i+1][aft], dp[i][j] + c[i]);
		}
	}

	ll ans = -1;
	if(dp[N][POW[K]-1] != INF64) ans = dp[N][POW[K]-1];
	cout << ans << endl;

	return 0;
}
