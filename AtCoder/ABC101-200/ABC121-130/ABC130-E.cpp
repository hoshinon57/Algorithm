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

// ABC130 https://atcoder.jp/contests/abc130

/*
 * DPおよび2次元累積和的な手法で解く。
 * 
 * まず、dp[i][j]を以下のように定義する。
 *   Sのi文字目、Tのj文字目まで見て、  (i:1～N, j:1～M)
 *   i,j文字目を共通整数列として"使うとき"に、
 *   条件を満たす組の数
 * 
 * するとdp[i][j]は、Sのi文字目、Tのj文字目が不一致ならば0.
 * 一致する場合、これまでの組に、末尾にi,j文字目を追加したものを増やせるので
 *   dp[i][j] = Σdp[x][y] (x:0～i-1, y:0～j-1)
 * となる。
 * dp[][]の全要素の総和が答となる。
 * 
 * ただし上記のΣ計算を都度行うと間に合わない。
 * そのため
 *   sum[i][j]：dp[0][0]～dp[i][j]までの累積和
 * を用意する。
 * すると dp[i][j] = sum[i-1][j-1] となる。
 * 
 * sumの構築は二次元累積和の考え方を用いて、
 *   sum[i][j] = sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + dp[i][j]
 * となる。
 * 
 * [ACまでの思考の流れ]
 * ・メタ読みで、制約でN,Mが10^3なのでO(NM)っぽい。
 *   よって各(i,j)の組はO(1)で処理したい。
 * ・i,j文字目に着目したとき、手前の文字列からどう遷移するかを考える。
 * ・dp[i][j]がどうやらそれより手前の要素を合算したものっぽいので、
 *   高速に計算するならば累積和を使えばよさそう。
 */

int main(void)
{
	int i, j;
	ll N, M; cin >> N >> M;
	vector<ll> s(N+1); for(i = 1; i <= N; i++) {cin >> s[i];}
	vector<ll> t(M+1); for(i = 1; i <= M; i++) {cin >> t[i];}

	vector<vector<ll>> dp(N+1, vector<ll>(M+1, 0));  // dp[N+1][M+1]

	// sum[i][j]：dp[0][0]～dp[i][j]までの累積和
	vector<vector<ll>> sum(N+1, vector<ll>(M+1, 0));  // sum[N+1][M+1]

	dp[0][0] = 1;  // 空集合のぶん
	// S,Tの片方を使わない部分は事前計算しておく
	for(i = 0; i <= N; i++) sum[i][0] = 1;
	for(j = 0; j <= M; j++) sum[0][j] = 1;

	for(i = 1; i <= N; i++)
	{
		for(j = 1; j <= M; j++)
		{
			if(s[i] == t[j]) dp[i][j] = sum[i-1][j-1];

			// 累積和の更新
			sum[i][j] = (sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + dp[i][j] + MOD) % MOD;  // 減算があるため、MODを足しておく
		}
	}
	cout << sum[N][M] << endl;  // dp[0][0]～dp[N][M]までの総和

	return 0;
}
