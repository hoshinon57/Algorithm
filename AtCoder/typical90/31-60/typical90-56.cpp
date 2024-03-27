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

// 競プロ典型90問:56 https://atcoder.jp/contests/typical90/tasks/typical90_bd

/*
 * DP復元で解く。ほぼ公式解説通りの実装だった。
 *   https://twitter.com/e869120/status/1400221404109230082
 * 
 * dp[i][j]を「i個目の福袋まで見たときに、総額がjになる買い方があればtrue」と定義する。
 * これは通常のDPで解ける。
 * 
 * dp[N][S]がfalseならImpossibleで終了。
 * trueなら、dp[N][S]から手前方向に探索していく。
 * ★dp[N][S]=trueから始めているので、必ず、A,Bどちらかには遷移できる。
 */

int main(void)
{
	int i, j;
	int N, S; cin >> N >> S;
	vector<int> a(N+1), b(N+1);
	for(i = 1; i <= N; i++) cin >> a[i] >> b[i];
	vector<vector<bool>> dp(N+1, vector<bool>(S+1, false));  // dp[N+1][S+1]
	dp[0][0] = true;
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= S; j++)
		{
			if(j-a[i]>=0 && dp[i-1][j-a[i]]) dp[i][j] = true;
			if(j-b[i]>=0 && dp[i-1][j-b[i]]) dp[i][j] = true;
		}
	}

	if(!dp[N][S])
	{
		cout << "Impossible" << endl;
		return 0;
	}
	string ans;
	// dp[N][S]から復元
	j = S;
	for(i = N; i > 0; i--)
	{
		if(j-a[i]>=0 && dp[i-1][j-a[i]])
		{
			ans += 'A';
			j -= a[i];
		}
		else  // A,Bどちらか片方には必ず遷移できる
		{
			ans += 'B';
			j -= b[i];
		}
	}
	reverse(ans.begin(), ans.end());
	cout << ans << endl;

	return 0;
}
