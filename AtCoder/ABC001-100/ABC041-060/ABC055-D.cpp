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

// ABC055 https://atcoder.jp/contests/abc055

/*
 * drken氏の以下の記述の通り。
 * ＞いくつかの値を決めると、残りが決まっていくので、最後に整合性をcheckする
 *   https://drken1215.hatenablog.com/entry/2024/09/22/143748
 * 
 * 隣り合った2匹の羊/狼が決まると、残りの動物も決めていける。
 * 最後まで決めて、最初に決め打ちした2匹に矛盾が無ければOK.
 * 
 * 実装でミスってWAしまくった…反省。
 * (決め打ちの2匹の設定をミスった)
**/

char rev(char c)
{
	return ((c=='S') ? 'W' : 'S');
}

int main(void)
{
	int N; cin >> N;
	string s; cin >> s;

	string ans;
	auto chk = [&](void) -> bool
	{
		auto bk = ans;
		for(int i = 0; i < N; i++)
		{
			int r = 0;  // 反転するなら1
			if(s[i] == 'x') r = 1;
			if(ans[i] == 'W') r = 1-r;
			ans[(i+1)%N] = (r ? rev(ans[(i-1+N)%N]) : ans[(i-1+N)%N]);
		}
		// 最初に決め打ちした2つが変わっていないか
		return (bk[0] == ans[0] && bk[N-1] == ans[N-1]);
	};
	ans.resize(N, 'S');
	for(int _1 = 0; _1 < 2; _1++)
	{
		ans[0] = (_1 == 0 ? 'S' : 'W');
		for(int _2 = 0; _2 < 2; _2++)
		{
			ans[N-1] = (_2 == 0 ? 'S' : 'W');
			auto bk = ans;
			if(chk())
			{
				cout << ans << endl;
				return 0;
			}
			ans = bk;
		}
	}
	cout << -1 << endl;

	return 0;
}
