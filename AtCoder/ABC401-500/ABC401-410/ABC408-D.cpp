#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC408 https://atcoder.jp/contests/abc408

/*
 * コンテスト時に解けなかった。
 * 現在の状態を情報に持つDP, 俗称 耳DP で解く。
 * 
 * 出来上がった文字列は 00...0011...1100...00 となることから、
 * 最初の0, 1, 2回目の0,の3フェーズに分けられる。
 * (最後まで見て、フェーズ1,2が存在しないこともある)
 * 
 * dp[i]を以下のように定義する。
 *   その文字列まで見て、現在の状態が
 *   i=0:最初の0, i=1:1, i=2:2回目の0
 *   であるときの、操作回数の最小値
 * 最後まで見て、dp[0]～dp[2]の最小値が答。(000..000や、000..0011, 111...111 といった形もありえるので)
 * 
 * フェーズは順に進んでいくので、
 * ndp[1]はmin(dp[0],dp[1])から求まる。またその文字が'0'であれば変更のため+1が必要。
 * ndp[0],ndp[2]も同様。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・これムズイなー…
 *   操作した最終形を見て、3つのフェーズに分かれていると見ることができるか。
 * ・「現在の状態を情報に持つDP」の問題を解いたことは何度かあるが、意識に定着していない。
 *   耳DPという俗称とともに覚えておく。
**/

void solve(void)
{
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	vector<ll> dp(3, INF64);
	dp[0] = 0;
	for(i = 0; i < N; i++)
	{
		vector<ll> ndp(3);

		ndp[0] = dp[0];
		if(s[i] == '1') ndp[0]++;

		ndp[1] = min(dp[0], dp[1]);
		if(s[i] == '0') ndp[1]++;

		ndp[2] = min(dp[1], dp[2]);
		if(s[i] == '1') ndp[2]++;

		swap(dp, ndp);
	}
	cout << *min_element(dp.begin(), dp.end()) << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T>0)
	{
		T--;
		solve();
	}

	return 0;
}
