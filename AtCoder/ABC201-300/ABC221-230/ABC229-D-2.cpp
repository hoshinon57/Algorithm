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

// ABC229 https://atcoder.jp/contests/abc229

// 尺取り法で解く版

int main(void)
{
	string s; cin >> s;
	int N = (int)s.size();
	int K; cin >> K;

	int cnt = 0;  // .をXに置き換えている個数
	// 今見ている区間にpos文字目を追加しても合法ならtrue
	auto check = [&](int pos) -> bool
	{
		if(s[pos] == '.')
		{
			return (cnt+1 <= K);
		}
		else
		{
			return (cnt <= K);
		}
	};
	int ans = 0;
	int l, r;
	for(l = 0, r = 0; l < N; l++)  // [l,r)
	{
		while(r < N && check(r))  // r文字目を追加しても大丈夫か
		{
			if(s[r] == '.') cnt++;
			r++;
		}
		chmax(ans, r-l);
		// l++するための準備
		if(l==r) r++;  // 例えば[3,3)が[4,3)になると破綻するので、rを加算しておく
		else
		{
			if(s[l] == '.') cnt--;
		}
	}
	cout << ans << endl;

	return 0;
}
