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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC104 https://atcoder.jp/contests/abc104

/*
 * 動的計画法で解く。けんちょん氏の解説が最も近かった。
 *   https://drken1215.hatenablog.com/entry/2018/08/05/224100_1
 * 
 * dp[*][i]を以下のように定義する。
 *   *文字目まで見て、
 *   "ABC"のうち i=0:まだ1文字もできていない, i=1:'A'まで作った, i=2:'AB'まで作った, i=3:'ABC'まで完成した
 *   ときの作り方の数
 * 最後まで見て、dp[*][3]が答。
 * 
 * 実装ではndp,dpとしている。
 * 
 * DP遷移は
 * 1)*文字目を使わないケース
 *   ndp=dp
 *   ただし'?'の場合はA,B,C3通り使えることから、ndp=dp*3とする。
 * 2)*文字目を使うケース
 *   'A'または'?'のとき、ndp[1]+=dp[0]
 *   B,Cも同様。
**/

int main(void)
{
	int i;
	string s; cin >> s;
	vector<mint> dp(4);
	dp[0] = 1;
	for(auto &e : s)
	{
		vector<mint> ndp(4);
		// x文字目を使わないケース
		ndp = dp;
		if(e == '?')
		{
			for(i = 0; i <= 3; i++) ndp[i] *= 3;
		}

		if(e == 'A' || e == '?') ndp[1] += dp[0];
		if(e == 'B' || e == '?') ndp[2] += dp[1];
		if(e == 'C' || e == '?') ndp[3] += dp[2];

		swap(ndp, dp);
	}

	cout << dp[3].val() << endl;

	return 0;
}
