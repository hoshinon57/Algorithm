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

// ABC344 https://atcoder.jp/contests/abc344

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   袋iまで見て、 (i=1～N)
 *   Tのj文字目まで作るときの、 (j=0～|T|)
 *   支払う金額の最小値  作れなければINF
 * 
 * 袋の文字ごとに、{Tの何文字目から, 一致する文字数} というのを計算しておく。
 * 同じ文字でも、Tの複数の位置で一致する可能性があることに注意。
 * 
 * するともらうDPにて、以下の遷移が可能となる。
 *   [pos,len] としたときに、
 *   dp[i-1][pos-1] に文字を付与すると dp[i][pos+len-1] となる。
 *   DP値は+1する(支払いは1円)。
 */

int main(void)
{
	// 1-indexed
	int i, j;
	string T; cin >> T;
	int N; cin >> N;
	vector<vector<pair<int,int>>> list(N+1);  // list[i] : Aiについて {合う先頭文字, 文字列長}
	for(i = 1; i <= N; i++)
	{
		int A; cin >> A;
		for(j = 0; j < A; j++)
		{
			string s; cin >> s;
			int pos = 0;
			// Tのどの位置に一致するかを探索 複数個所で一致する可能性がある点に注意
			while(true)
			{
				auto p = T.find(s, pos);
				if(p == string::npos) break;
				pos = (int)p+1;
				list[i].push_back({pos, s.size()});
			}
		}
	}

	vector<vector<int>> dp(N+1, vector<int>(T.size()+1, INF32));  // dp[N+1][T.size()+1]
	dp[0][0] = 0;
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j <= (int)T.size(); j++)
		{
			dp[i][j] = dp[i-1][j];  // 袋iを使わない
		}
		for(auto &e : list[i])
		{
			auto [pos, len] = e;
			int d = dp[i-1][pos-1] + 1;
			chmin(dp[i][pos+len-1], d);
		}
	}
	int ans = dp[N][T.size()];
	if(ans >= INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
