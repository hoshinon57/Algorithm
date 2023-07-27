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

/*
 * メモ化再帰で解く。
 * 
 * dp[i][j]の定義は以下。
 *   残りの石がi個で、
 *   次が{j=0:高橋, j=1:青木}のターンのときに、
 *   高橋くんが取れる石の最大数
 * 
 * func(i,j)をdp[i][j]を返す関数として用意し、内部ではメモ化再帰を用いる。
 * DP遷移のアルゴリズムはABC270-D.cppと同様。
 * (実装時期が半年以上異なるので、コードの雰囲気は大きく異なるが)
 */

int dp[10005][2];

vector<int> a;

// dp[i][j]をメモ化再帰で解いて返す
int func(int i, int j)
{
	if(i == 0) return 0;  // 残りの石は0個
	if(dp[i][j] != -1) return dp[i][j];  // メモ済み

	if(j == 0)  // 高橋くんのターン
	{
		int res = 0;
		for(auto &e : a)
		{
			if(i < e) continue;  // e個は残ってない
			res = max(res, e + func(i-e, 1));
		}
		dp[i][0] = res;
	}
	else  // 青木くんのターン
	{
		int res = INF32;
		for(auto &e : a)
		{
			if(i < e) continue;  // e個は残ってない
			res = min(res, func(i-e, 0));
		}
		dp[i][1] = res;
	}

	return dp[i][j];
}

int main(void)
{
	// 0-indexed
	int i;
	int N, K;
	cin >> N >> K;
	a.resize(K);
	for(i = 0; i < K; i++)
	{
		cin >> a[i];
	}
	// DP初期設定 メモ未実施として-1を設定
	for(i = 1; i <= N; i++)
	{
		dp[i][0] = dp[i][1] = -1;
	}

	cout << func(N, 0) << endl;

	return 0;
}
