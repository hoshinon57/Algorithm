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

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * dp[i][j]を以下のように定義する。
 *   sのi文字目まで、tのj文字目まで見たときの最長共通部分列の長さ
 *   (i=1～|S|, j=1～|T|)
 * 遷移は、s[i]=t[j]ならdp値を+1しつつ右下に進み、
 * そうでなければdp値は据え置きで右および下に進むイメージ。
 * 
 * 復元は以下のように行う。
 * ・今見ている文字が一致していれば、その文字を採用し、左上に進む
 * ・不一致であれば、文字は採用せず、上または左のうちdp値が同一の方向に進む
 *   (同一となる方向が必ず存在する)
 */

int main(void)
{
	int i, j;
	string s, t; cin >> s >> t;
	int sl = (int)s.size();
	int tl = (int)t.size();
	vector<vector<int>> dp(sl+1, vector<int>(tl+1, -INF32));  // dp[sl+1][tl+1]
	dp[0][0] = 0;
	for(i = 0; i <= sl; i++)
	{
		for(j = 0; j <= tl; j++)
		{
			if(i-1 >= 0) chmax(dp[i][j], dp[i-1][j]);
			if(j-1 >= 0) chmax(dp[i][j], dp[i][j-1]);
			if(i-1 >= 0 && j-1 >= 0)
			{
				if(s[i-1] == t[j-1]) chmax(dp[i][j], dp[i-1][j-1]+1);
			}
		}
	}

	// DP復元
	string ans;
	i = sl; j = tl;
	while(i>=0 && j>=0)
	{
		// 文字が同じなら採用
		if(i-1 >= 0 && j-1 >= 0 && s[i-1] == t[j-1])
		{
			ans.push_back(s[i-1]);
			i--; j--;
		}
		else
		{
			// dp値が同一の方へ遷移
			// 必ずどちらかへは遷移できる
			if(i-1 >= 0 && dp[i][j] == dp[i-1][j]) i--;
			else j--;
		}
	}
	std::reverse(ans.begin(), ans.end());
	std::cout << ans << endl;

	return 0;
}
