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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題38
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_10_C&lang=ja

/*
 * 動的計画法(もらうDP)で解く。
 * いわゆる「教科書通りの実装」であり、説明は省略する。
 * 
 * 参考：けんちょん氏の本のcode5.8
 */

void solve(void)
{
	int i, j;
	string s, t; cin >> s >> t;

	// dp[i][j]	
	//   sの先頭i文字と、  (i:0～s.size)
	//   tの先頭j文字までを見たときに、  (j:0～t.size)
	//   それまでの最長共通部分列
	vector<vector<int>> dp(s.size()+1, vector<int>(t.size()+1, -INF32));  // dp[s.size()+1][t.size()+1]
	dp[0][0] = 0;

	for(i = 0; i <= (int)s.size(); i++)
	{
		for(j = 0; j <= (int)t.size(); j++)
		{
			if(i == 0 && j == 0) continue;
			int t1 = -INF32, t2 = -INF32, t3 = -INF32;

			// 左方向、右方向からの遷移
			// 片方の文字列からのみ進めるのであれば、共通部分列は変化しない
			if(j-1 >= 0) t1 = dp[i][j-1];
			if(i-1 >= 0) t2 = dp[i-1][j];

			// 左上方向からの遷移
			// 文字列が一致する場合、共通部分列は1増加する
			if(j-1 >= 0 && i-1 >= 0)
			{
				t3 = dp[i-1][j-1];
				if(s[i-1] == t[j-1]) t3++;
			}
			dp[i][j] = max({t1, t2, t3});
		}
	}
	cout << dp[s.size()][t.size()] << endl;
}

int main(void)
{
	int Q; cin >> Q;
	while(Q > 0)
	{
		solve();
		Q--;
	}

	return 0;
}
