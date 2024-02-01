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

// ABC195 https://atcoder.jp/contests/abc195

/*
 * 自力で解けず、解説を見た。動的計画法で解く。
 *   https://atcoder.jp/contests/abc195/editorial/886
 * 
 * dp[i][j]を以下のように定義する。
 *   iラウンド目まで終わった段階で、 (i=1～N)
 *   Tを7で割った余りがjであるときに、 (j=0～6)
 *   高橋君が勝てるならtrue, 負けるならfalse
 * 「全Nラウンドが終わった後でTが7の倍数なら勝ち」なので、dp[N][0]=trueが初期値となる。
 * 1ラウンド目が始まる前、dp[0][0]のt/fが答となる。
 * 
 * 遷移は以下のようになる。
 * iラウンド目が高橋ターンのとき
 *   Siまたは0を加えるパターンのうち、どちらかの遷移先がtrueなら、高橋くんは勝てる。
 *   つまり(i-1)ラウンド目が終わったときのTについて、
 *   dp[i][(T*10+j)%7]=true または dp[i][T*10%7]=true ならばdp[i-1][j]=trueとなる。
 * iラウンド目が青木ターンのとき
 *   Siまたは0を加えるパターンのうち、両方の遷移先がtrueなら、高橋くんは勝てる。
 *   つまり(i-1)ラウンド目が終わったときのTについて、
 *   dp[i][(T*10+j)%7]=true かつ   dp[i][T*10%7]=true ならばdp[i-1][j]=trueとなる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・ゲームは後ろから考える。
 *   …はすぐ思い浮かんだものの、そこからdp配列を作れなかった。
 * ・DPとしては、「何ラウンド目まで終わったか」「そこまでのTを7で割った余り」を情報として持ちたい。
 * ・勝利条件が分かっているので、それに合うDP初期値を設定する。
 *   今回なら「Nラウンド目まで終わって、Tを7で割った余りが0」なので、dp[N][0]=true.
 *   そこから前へ前へと遷移していく。
 * ・Tの初期値は0、最初は「0ラウンド目まで終わった段階」なので、dp[0][0]が答となる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	string s, x; cin >> s >> x;
	s = '_' + s;
	x = '_' + x;
	vector<vector<bool>> dp(N+1, vector<bool>(7, false));
	dp[N][0] = true;
	for(i = N-1; i >= 0; i--)  // iラウンド目が終わった後について考える -> i+1ラウンド目のs,xを見る
	{
		for(j = 0; j < 7; j++)
		{
			int sn = s[i+1] - '0';
			int nxt1 = (j*10 + sn)%7;
			int nxt2 = j*10 % 7;
			if(x[i+1] == 'T')
			{
				if(dp[i+1][nxt1] || dp[i+1][nxt2]) dp[i][j] = true;
			}
			else
			{
				if(dp[i+1][nxt1] && dp[i+1][nxt2]) dp[i][j] = true;
			}
		}
	}
	string ans = "Takahashi";
	if(!dp[0][0]) ans = "Aoki";
	cout << ans << endl;

	return 0;
}
