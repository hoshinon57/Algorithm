#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC262 https://atcoder.jp/contests/abc262

/*
 * 動的計画法で解く。
 * 
 * 「選んだ項の平均値が整数」とは、「選んだ項の総和を項数で割った余りが0」ということ。
 * このとき、数列Aから何個選ぶか、は1～NのN通りとなる。
 * 
 * dp[i][j][k]を以下で定義する。
 *   数列Aのa(i)まで見たときに、   i:1～N
 *   そこまでにj個選んでいて、     j:0～N
 *   そこまでの選んだ項の総和を、Xで割った余りがk   k:0～X-1
 *   のときの場合の数
 * Xは1～Nまでそれぞれを計算する。
 * 
 * DPの遷移は、配るDPにてa(i+1)を選ぶか選ばないかで変わる。
 * ・a(i+1)を選ぶ場合
 *   選ぶ個数は+1となり、
 *   総和をXで割った余りは(k+a[i+1])%Xとなるので、
 *   dp[i+1][j+1][(k+a[i+1])%x] <- dp[i][j][k]
 * ・a(i+1)を選ばない場合
 *   選ぶ個数も総和も変わらないので、
 *   dp[i+1][j][k] <- dp[i][j][k]
 * 
 * 上記をX:1～Nまでそれぞれ計算し、総和を求めればよい。
 */

const ll MOD = 998244353;
ll N;
vector<ll> a;  // 1-indexed
ll answer = 0;

// num:Aからnum個選ぶ
ll solve(ll num)
{
	ll i, j, k;

	// dp[i][j][k]:
	//   数列Aのa(i)まで見たときに、   i:1～N
	//   そこまでにj個選んでいて、     j:0～N
	//   そこまでの選んだ項の総和を、numで割った余りがk   k:0～X-1
	// のときの場合の数
	vector<vector<vector<ll>>> dp(N+1, vector<vector<ll>>(N+1, vector<ll>(num)));
	dp[0][0][0] = 1;  // DP初期値 0個選んでいる状態

	// 配るDP
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			for(k = 0; k < num; k++)
			{
				if(dp[i][j][k] == 0) continue;  // ここに遷移することは無いのでスルー

				// 選ぶ場合
				ll tmp = (k+a[i+1])%num;
				dp[i+1][j+1][tmp] += dp[i][j][k];
				dp[i+1][j+1][tmp] %= MOD;

				// 選ばない場合
				dp[i+1][j][k] += dp[i][j][k];
				dp[i+1][j][k] %= MOD;
			}
		}
	}

	ll ret = 0;
	// 最終的に、数列Aからnum個選んで
	// 総和をnumで割った余りが0になるものを求めたい
	ret = dp[N][num][0];

	return ret;
}

int main(void)
{
	// 1-indexed
	ll i;
	cin >> N;
	a.resize(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> a[i];
	}

	for(i = 1; i <= N; i++)
	{
		answer += solve(i);
		answer %= MOD;
	}
	cout << answer << endl;
	
	return 0;
}
