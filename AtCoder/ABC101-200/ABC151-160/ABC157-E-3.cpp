#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC157 https://atcoder.jp/contests/abc157

/*
 * 文字ごとに登場位置を管理して解く版。
 * 文字が変化するため、vectorではなくsetにて管理する必要がある。
 * 
 * ※最初、「区間をsetで管理」のライブラリで解いたが、そこまで使う必要は無かった。単なるsetで良かった。
 *   https://atcoder.jp/contests/abc157/submissions/61133905
 * 
 * クエリは、lower_bound(l)にて得たイテレータの値がr以下ならOK.
 * これを'a'～'z'までそれぞれチェックする。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	string s; cin >> s;
	int Q; cin >> Q;
	vector<set<int>> sl(26);  // sl[0]:'a'の登場位置
	for(i = 0; i < N; i++)
	{
		sl[s[i]-'a'].insert(i);
	}
	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			int pos; cin >> pos;
			pos--;
			char c; cin >> c;
			// 古いのを消す
			sl[s[pos]-'a'].erase(pos);
			s[pos] = c;
			// 新しいのを追加
			sl[c-'a'].insert(pos);
		}
		else
		{
			int l, r; cin >> l >> r;
			l--; r--;
			int ans = 0;
			for(i = 0; i < 26; i++)
			{
				auto itr = sl[i].lower_bound(l);
				if(itr != sl[i].end() && *itr <= r) ans++;
			}
			cout << ans << endl;
		}
	}

	return 0;
}
