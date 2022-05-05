#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC049 https://atcoder.jp/contests/abc049/
// AtCoder Beginners Selectionより
// 動的計画法で解く版

/*
 * 文字列操作にて、Sのピッタリi文字目までを生成可能ならdp[i]=1とする。
 * dp[0]=1からスタートし、
 * dp[]=1となっている箇所に対して次の文字列探索を進めていく。
 * 最後にdp[S.size()]が結果となる。
 */
int main(void)
{
	int i, j;
	string S;
	cin >> S;
	size_t len = S.size();
	
	// dp[0]～dp[len]
	// dp[i]:操作にて、Sの(ピッタリ)i文字目までを生成可能なら1
	vector<int> dp(len+1, 0);
	dp[0] = 1;  // 0文字目まで、は生成可能とする

	string word[4] = {"dream", "dreamer", "erase", "eraser"};
	for(i = 0; i < len; i++)
	{
		if(dp[i] == 0) continue;  // i文字目までが生成不可能なら次へ
		for(j = 0; j < 4; j++)
		{
			// Sの(i+1)文字目から一致する文字列があるか
			if(S.substr(i, word[j].size()) == word[j])
			{
				dp[i+word[j].size()] = 1;
			}
		}
	}

	if(dp[len] == 1)
	{
		cout << "YES" << endl;
	}
	else
	{
		cout << "NO" << endl;
	}

	return 0;
}
