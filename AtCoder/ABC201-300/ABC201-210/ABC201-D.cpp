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

// ABC201 https://atcoder.jp/contests/abc201

/*
 * 動的計画法で解く。(ゲームのDP)
 * メモ化再帰になっていたりもするが、考え方は以下の解説と同じ。
 *   https://atcoder.jp/contests/abc201/editorial/480
 *   https://blog.hamayanhamayan.com/entry/2021/05/15/235741
 * 
 * dp[i][j]を以下のように定義する。
 *   (i,j)マスから移動するときの(※)、
 *   高橋くんのターンなら：(高橋スコア)-(青木スコア) の最大値
 *   青木くんのターンなら：(高橋スコア)-(青木スコア) の最小値
 *   (※)(i,j)マスに到達したときのスコア増減は計算済み
 * dp[0][0]の正負で答が決まる。
 * 
 * i+jの偶奇で、次がどちらのターンかが分かる。
 * ゲームのDPなので終端から求めていく。
 */

int main(void)
{
	int i, j;
	int h, w; cin >> h >> w;
	vector<vector<int>> a(h, vector<int>(w, 0));  // a[h][w]
	for(i = 0; i < h; i++)
	{
		string s; cin >> s;
		for(j = 0; j < w; j++)
		{
			if(s[j] == '+') a[i][j] = 1;
			else a[i][j] = -1;
		}
	}
	a[0][0] = 0;

	vector<vector<int>> dp(h, vector<int>(w, 0));  // dp[h][w]
	// dp[h-1][w-1] = 0;
	for(i = h-1; i >= 0; i--)
	{
		for(j = w-1; j >= 0; j--)
		{
			if(i == h-1 && j == w-1) continue;  // 最終マス
			if((i+j)%2 == 0)  // Takahashi move
			{
				int s = -INF32;  // maxを求めたいので、初期値は負のINF
				if(i != h-1)  // 下へ移動
				{
					s = max(s, dp[i+1][j] + a[i+1][j]);
				}
				if(j != w-1)  // 右へ移動
				{
					s = max(s, dp[i][j+1] + a[i][j+1]);
				}
				dp[i][j] = s;
			}
			else  // Aoki move
			{
				int s = INF32;
				if(i != h-1)  // 下へ移動
				{
					s = min(s, dp[i+1][j] - a[i+1][j]);  // (高橋スコア)-(青木スコア) がDPの定義なので、青木ターンは減算
				}
				if(j != w-1)  // 右へ移動
				{
					s = min(s, dp[i][j+1] - a[i][j+1]);
				}
				dp[i][j] = s;
			}
		}
	}
	string ans = "Draw";
	if(dp[0][0] > 0) ans = "Takahashi";
	else if(dp[0][0] < 0) ans = "Aoki";
	cout << ans << endl;

	return 0;
}
