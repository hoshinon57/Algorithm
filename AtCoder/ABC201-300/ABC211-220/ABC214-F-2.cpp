#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC214 https://atcoder.jp/contests/abc214

// map使用で解いてみた

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

int main(void)
{
	int i;
	string s; cin >> s;
	s = "**" + s;  // 先頭に番兵2つ
	ll len = s.size();
	vector<mint> dp(len+5), dp_sum(len+5);
	dp[0] = 1;
	dp[1] = 0;
	dp_sum[1] = dp_sum[0] + dp[0];
	dp_sum[2] = dp_sum[1] + dp[1];
	map<char,int> pre;  // pre[c]:文字cが前回登場した位置 =0なら1に補正する

	for(i = 2; i < len; i++)
	{
		int p = pre[s[i]];
		if(p == 0) p = 1;
		// [pre[c]-1], i-2] の総和 -> [pre[c]-1], i-1)
		dp[i] = dp_sum[i-1] - dp_sum[p-1];
		dp_sum[i+1] = dp_sum[i] + dp[i];

		pre[s[i]] = i;
	}

	// [2,len) の総和
	mint ans = dp_sum[len] - dp_sum[2];
	cout << ans.val() << endl;

	return 0;
}
