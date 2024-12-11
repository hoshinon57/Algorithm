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

// https://atcoder.jp/contests/sumitrust2019/tasks/sumitb2019_c

/*
 * 各種解説を元に、DPで解くのと買う個数を全探索の2通り実装してみた。
 * 
 * DPはdp[i]:i円となる買い方があるか、で配るDP.
 * 
 * 買う個数を全探索は、6種の価格の100円未満の端数に着目する。
 * 例えば5個買うなら、端数部分が0円～25円の全パターンで買うことができる。
 */

int main(void)
{
#if 1
	// DPで解く
	int i;
	int X; cin >> X;
	vector<bool> dp(X+200, false);  // dp[i]:i円となる買い方があるか
	dp[0] = true;
	// 配るDP
	for(i = 0; i <= X-100; i++)
	{
		if(!dp[i]) continue;
		for(int j = 100; j <= 105; j++) dp[i+j] = true;
	}
	int ans = (dp[X]) ? 1 : 0;
	cout << ans << endl;
#else
	// 買う個数を全探索
	int X; cin >> X;
	int ans = 0;
	for(int i = 1; i <= 1000; i++)  // 全部でi個買う
	{
		int rem = X - i*100;
		if(rem >= 0 && rem <= i*5) ans = 1;
	}
	cout << ans << endl;
#endif

	return 0;
}
