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

// ABC375 https://atcoder.jp/contests/abc375

/*
 * 累積和と真ん中固定の典型で解く。公式解説の通り。
 *   https://atcoder.jp/contests/abc375/editorial/11139
 * 
 * Sjに着目すると、Si=Skより
 *   ある文字について、 (26通り)
 *   jより左側にあるその文字数をl,
 *   jより右側にあるその文字数をrとすると、
 *   l*rがSjにおける回文の数となる。
 * 
 * 各文字について累積和を事前計算すると、
 * lは[0,j), rは[j+1,len) の範囲となり、O(1)で求められる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・i<j<kについて考える場合、jに着目する。[典型]
 */

int main(void)
{
	int i, j;
	string s; cin >> s;
	int len = (int)s.size();
	vector<vector<int>> sum(26, vector<int>(len+1, 0));  // sum[26][len+1]
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < 26; j++)
		{
			sum[j][i+1] = sum[j][i];
		}
		sum[s[i]-'A'][i+1]++;
	}

	ll ans = 0;
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < 26; j++)
		{
			// 左側 [0,i), 右側 [i+1,len)
			ll l = sum[j][i];
			ll r = sum[j][len] - sum[j][i+1];
			ans += l*r;
		}
	}
	cout << ans << endl;

	return 0;
}
