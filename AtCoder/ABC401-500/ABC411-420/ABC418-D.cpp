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

// ABC418 https://atcoder.jp/contests/abc418

/*
 * ※公式解説では「操作で0の数の偶奇は変わらない」という不変量をベースとしていた。なるほど。
 *   以下は別解法。
 * 
 * 手作業で試してみると、最後に残る文字は操作順によらないっぽい、というのが分かる。
 * ⇒XNORは結合法則(※)が成り立つとのこと。
 *   (※)a+(b+c)=(a+b)+c みたいなやつ
 *   https://x.com/takytank/status/1954177485639487809
 *   https://x.com/naoya_ito/status/1954176696946454732
 *   一方でNORやNANDは結合法則は成り立たないらしい。
 *   https://nikoniko.shinoarchive.com/no-hint-redstone-2/
 * 
 * 部分文字列ということで、区間[l,r]についてrで全探索することを考えてみる。
 * dp[i][j]を以下のように定義する。
 *   区間の右端がiで、そこまでのXNORがjとなるような、組み合わせの数
 * Σdp[*][1]が答。
 * 
 * DP遷移は、もらうDPにて
 * Sのi文字目が'0'のとき：
 *   dp[i][0] = dp[i-1][1];
 *   dp[i][1] = dp[i-1][0];
 *   dp[i][0]++;  // 区間[i,i]の部分
 * となる。
 * i文字目が'1'のときも同様。
 * 
 * 実装ではdp,ndp形式としている。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	string s; cin >> s;

	vector<ll> dp(2);
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		vector<ll> ndp(2);
		if(s[i] == '0')
		{
			ndp[0] = dp[1];
			ndp[1] = dp[0];
			ndp[0]++;  // 区間[i,i]の部分
		}
		else
		{
			ndp[0] = dp[0];
			ndp[1] = dp[1];
			ndp[1]++;
		}
		ans += ndp[1];
		swap(dp, ndp);
	}
	cout << ans << endl;

	return 0;
}
