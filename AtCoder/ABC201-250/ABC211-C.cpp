#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int MOD = 1000000007;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC211 https://atcoder.jp/contests/abc211

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   Sのi文字目まで見たときに、
 *   パターンのj文字目まで下線を引くときの
 *   組み合わせの数
 * 
 * 初期値はdp[0][0]=1で、遷移は以下となる。
 * (1)i文字目に下線を引かない場合
 *   dp[i][j] = dp[i-1][j]
 * (2)i文字目に下線を引く（引ける）場合
 *   (1)に, dp[i-1][j-1] を加算
 * 
 * S,patternともに、先頭に"chokudai"以外の文字を入れておくと、処理が簡易になる。
 * dp[S][pattern]が答。
 */

int main(void)
{
	size_t i, j;
	string S;
	cin >> S;
	// 先頭に番兵"-"を付与しておく
	S = "-" + S;
	string pat = "-" "chokudai";

	vector<vector<int>> dp(S.size(), vector<int>(pat.size(), 0));  // dp[S][pat]
	dp[0][0] = 1;

	for(i = 1; i < S.size(); i++)
	{
		for(j = 0; j < pat.size(); j++)
		{
			dp[i][j] = dp[i-1][j];  // i文字目に下線を引かない場合
			if(S[i] == pat[j])  // i文字目に下線を引く場合
			{
				dp[i][j] += dp[i-1][j-1];
				dp[i][j] %= MOD;
			}
		}
	}
	cout << dp[S.size()-1][pat.size()-1] << endl;

	return 0;
}
