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
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC214 https://atcoder.jp/contests/abc214

/*
 * 自力で解けず、解説を見た。動的計画法で解く。
 *   https://x.com/kyopro_friends/status/1426561741731684353
 *   https://www.youtube.com/watch?v=RCB7EZaddEQ
 * 
 * 取り出す文字の位置が異なるのに、結果として同じ文字列ができるケースが厄介。
 * この重複をどう除外するか。
 * 
 * 考え方を変えて、S="abaccab"という入力から"acb"を取り出したいとする。
 * 複数パターンの取り出し方があるが、
 *   先頭から貪欲に取っていく (上記例ならS[0],[3],[6])
 * というやり方にて、実は重複なく数え上げることができる。
 * 
 * dp[i]を以下のように定義する。
 *   i番目の文字まで見て、i番目の文字を使うときの、作れる文字列の種類
 * これにより、前述の取り出し方にて数えることができる。
 * 
 * dp[i]への遷移は、連続した2文字は選べないことから、
 * s[i]=s[x]となるiの直前の位置xを求めて、dp[x-1]～dp[i-2]までの総和となる。
 * 
 * 番兵として、先頭に2文字追加しておくと処理が楽。
 * ただし、空文字としてdp[0]=1になるのは自然だが、dp[1]=0にする必要がある。 (連続した2文字は選べない観点で)
 * ここが自力で実装しきれなかった。
 * 
 * 実装は二重ループに見えるが、O(N^2)にはならない。
 * 文字数をKとして、文字種ごとにO(N)であることから、全体でもO(KN)に抑えられるため。
 * 詳細はYouTubeの解説を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・文字列に関する数え上げで、どうすれば重複が発生しない数え方になるかがポイント。
 *   逆に「ある特定の文字列を作りたい」ときに、先頭から貪欲に選ぶようにすれば重複なく数えられる。
 *   これを実現するにはdp[i]を「i文字目を選ぶ」という条件にすればよい。
 *   (これまで解いた問題だと、文字の種類ごとに考えるという手法があった)
 * ・番兵を用いるにあたり、本問では2文字番兵を入れる必要がある。
 *   またこの場合、DP初期値にも注意。
 * 
 * [その他]
 * ・DP遷移にて累積和を使う実装も試したが、実行時間は変わらなかった(むしろ増えた)。
 * ・jに関するループを2回ではなく1回に済ます実装も試したが、直感的にしっくりこなかった。これは実力不足だと思うが。
 */

int main(void)
{
	int i, j;
	string s; cin >> s;
	s = "**" + s;  // 先頭に番兵を2文字
	int len = (int)s.size();
	vector<ll> dp(len);
	dp[0] = 1;  // dp[1]=0である点に注意

	ll ans = 0;
	for(i = 2; i < len; i++)
	{
#if 1
		// s[i]より手前で、s[i]=s[x]となる位置xを求める ただしxは下限1でクリップ
		// [x-1, i-2]の区間のdp[*]の総和がdp[i]となる
		int x;
		for(x = i-1; x > 1; x--) if(s[i] == s[x]) break;
		for(j = x-1; j <= i-2; j++) dp[i] += dp[j];
#else  // ループを1回で抑える場合はこちら
		for(j = i-2; j >= 0; j--)
		{
			dp[i] += dp[j];
			if(s[i] == s[j+1]) break;
		}
#endif
		dp[i] %= MOD;
		ans += dp[i];
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
