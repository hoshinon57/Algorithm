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

// ABC301 https://atcoder.jp/contests/abc301

/*
 * コンテスト中に解けず、実装バグでドハマリした問題。
 * 「2つの列が一致」を、文字数のカウントで判定する。
 * 
 * まず、入力文字列を文字ごとにカウントする。
 * 実装では、S側のカウントをcnt[0][*], T側をcnt[1][*]とした。
 * 
 * 以下の方針で判定すればよい。
 * 文字aからzまでそれぞれ見ていき、
 *   cnt[0][*]==cnt[1][*]なら次へ。
 *   cnt[0][*]!=cnt[1][*]のとき、
 *     @で置き換えられる文字でなければ、NG.
 *     @で置き換えられる文字であれば、
 *       cnt[0], [1]のうち少ない方の@を使う。@が余っていなければNG.
 * NGにならずに最後まで到達できれば、OK.
 */

int cnt[2][26+1];  // 入力文字のカウント  cnt[S or T][A-Z or @]  @は[26]

bool solve(void)
{
	int i;
	string s, t; cin >> s >> t;
	for(i = 0; i < (int)s.size(); i++)
	{
		if(s[i] != '@') cnt[0][(int)(s[i]-'a')]++;
		else cnt[0][26]++;
		if(t[i] != '@') cnt[1][(int)(t[i]-'a')]++;
		else cnt[1][26]++;
	}

	string atcoder = "atcoder";
	for(i = 0; i < 26; i++)  // a-z文字目
	{
		int diff = cnt[0][i] - cnt[1][i];
		if(diff == 0) continue;  // OK

		// "atcoder"の文字でなければ、@を使えないためNG
		if(atcoder.find((char)(i+'a')) == string::npos) return false;

		if(diff > 0)  // [1]側の@を使う
		{
			if(cnt[1][26] >= diff)
			{
				cnt[1][26] -= diff;
			}
			else return false;  // @が余っていない NG
		}
		else  // [0]側の@を使う
		{
			diff = -diff;
			if(cnt[0][26] >= diff)
			{
				cnt[0][26] -= diff;
			}
			else return false;
		}
	}

	return true;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
