#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC185 https://atcoder.jp/contests/abc185

/*
 * 動的計画法で解く。
 * 最長共通部分列(LCS)の考え方で解ける。
 * 
 * dp[i][j]を以下のように定義する。
 *   A[i],B[j]までで共通部分列を作ろうとしたときに、
 *   A,Bから個別に削除した文字数と、A[x]!=B[y]となる文字数の和  (うまく書けん…)
 * 
 * A側だけ、またはB側だけ増やす場合は+1,
 * A,B同時に増やす場合、A,Bが同じ値ならば+0, 異なる値ならば+1,
 * とする。
 * dp[N][M]が答。
 */
int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<int> b(M+1); for(i = 1; i <= M; i++) {cin >> b[i];}

	vector<vector<int>> dp(N+1, vector<int>(M+1, INF32));  // dp[N+1][M+1]
	dp[0][0] = 0;

	// もらうDP
	for(i = 0; i <= N; i++)  // a[i]
	{
		for(j = 0; j <= M; j++)  // b[j]
		{
			if(i == 0 && j == 0) continue;
			int &d = dp[i][j];
			if(i-1 >= 0) d = min(d, dp[i-1][j]+1);  // 右方向
			if(j-1 >= 0) d = min(d, dp[i][j-1]+1);  // 下方向
			if(i-1 >= 0 && j-1 >= 0)  // 右下方向
			{
				if(a[i] == b[j])  d = min(d, dp[i-1][j-1]);  // 同じ値なので、増やさない
				else  d = min(d, dp[i-1][j-1]+1);
			}
		}
	}
	cout << dp[N][M] << endl;

	return 0;
}
