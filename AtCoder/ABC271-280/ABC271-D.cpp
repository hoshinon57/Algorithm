#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC271 https://atcoder.jp/contests/abc271

/*
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   i枚目までカードを置いたときに、 (i:1～N)
 *   それまでの総和をjにすることが、 (j:0～S)
 *   可能かどうか true/false で持つ。
 * 
 * 遷移式は、配るDPにて、
 * dp[i][j]がtrueであれば、
 *   dp[i+1][j+a[i+1]] = true  // i+1枚目のカードを、表で出すケース
 *   dp[i+1][j+b[i+1]] = true  // i+1枚目のカードを、裏で出すケース
 * となる。
 * 
 * 出力のYes/Noはdp[N][S]で判定する。
 * Yesの場合、カードの出し方の一例を提示する必要があり、以下2案がある。
 * (1)dpとは別の配列dp2を用意する。型はstring.
 *    DP遷移式にて、表裏に応じて末尾に'H'or'T'の文字を追加する。
 *    最後にdp2[N][S]を出力すればよい。
 *    ※dp2[i][j]は途中で別の文字列に上書きされることがあるが、
 *      「一例を求めよ」のため問題ない。
 * (2)末尾dp[N][S]から復元していく。
 *    dp[N][S]がtrueならば、dp[N-1][S-a[N]]またはdp[N-1][S-b[N]]の
 *    どちらかは必ずtrueであるため、trueになるものをたどっていく。
 *    
 * 以下のコードは(1)にて動作する。
 * (2)はソース後半の復元処理の部分を有効化すれば、動作する。
 */
int main(void)
{
	int i, j;
	int N, S;
	cin >> N >> S;
	vector<int> a(N+1), b(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> a[i] >> b[i];
	}

	// dp[i][j]
	// i枚目までカードを置いたときに、 (i:1～N)
	// それまでの総和をjにすることが、 (j:0～S)
	// 可能かどうか
	vector<vector<bool>> dp(N+1, vector<bool>(S+1, false));
	dp[0][0] = true;  // 初期設定

	// dp2[i][j]
	// dp[i][j]がtrueのとき、それまでの表裏の出し方の一例
	vector<vector<string>> dp2(N+1, vector<string>(S+1));

	// 配るDP
	for(i = 0; i < N; i++)
	{
		for(j = 0; j <= S; j++)
		{
			if(dp[i][j])
			{
				if(j+a[i+1] <= S)  // 表
				{
					dp [i+1][j+a[i+1]] = true;
					dp2[i+1][j+a[i+1]] = dp2[i][j] + 'H';
				}
				if(j+b[i+1] <= S)  // 裏
				{
					dp [i+1][j+b[i+1]] = true;
					dp2[i+1][j+b[i+1]] = dp2[i][j] + 'T';
				}
			}
		}
	}

#if 0  // 復元処理  出力部分と合わせて有効化すること
	string answer;
	int sum = S;
	for(i = N; i >= 1; i--)
	{
		// i枚目のカードを表で出したときに、復元できるか
		if(sum >= a[i] && dp[i-1][sum-a[i]])
		{
			answer += 'H';
			sum -= a[i];
		}
		// 表で出せない場合は、必ず裏で出せる
		else
		{
			answer += 'T';
			sum -= b[i];
		}
	}
	// 末尾から探索するため、反転
	std::reverse(answer.begin(), answer.end());
#endif

	if(dp[N][S])  // 総和をSにすることが可能
	{
		cout << "Yes" << endl;
#if 1
		cout << dp2[N][S] << endl;
#else  // 復元処理で作った文字列を使う場合、else側を有効にする
		cout << answer << endl;
#endif
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
