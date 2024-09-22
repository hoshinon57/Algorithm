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

// ABC372 https://atcoder.jp/contests/abc372

// x文字目を書き換えると、文字列"ABC"の探索について影響するのは
// (x-2)文字目スタート, (x-1)文字目スタート, x文字目スタートの3パターンのみ。
// ここの変化を差分として求めればよい。
// 番兵として、先頭末尾に2文字ずつ追加した。

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	string s; cin >> s;
	s = ".." + s + "..";  // 番兵
	int ans = 0;
	for(i = 0; i <= (int)s.size()-3; i++)
	{
		if(s.substr(i, 3) == "ABC") ans++;
	}
	while(Q > 0)
	{
		Q--;
		int x; cin >> x;
		x = x-1+2;  // 0-indexedへの変換と、番兵の分
		char c; cin >> c;

		int bef = 0, aft = 0;
		for(i = x-2; i <= x; i++)  // i文字目からチェック
		{
			if(s.substr(i, 3) == "ABC") bef++;
		}
		s[x] = c;
		for(i = x-2; i <= x; i++)  // i文字目からチェック
		{
			if(s.substr(i, 3) == "ABC") aft++;
		}
		ans += (aft-bef);
		cout << ans << endl;
	}

	return 0;
}
