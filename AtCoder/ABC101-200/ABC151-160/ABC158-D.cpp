#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <deque>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC158 https://atcoder.jp/contests/abc158

/*
 * 「S全体を反転する」といったのは愚直にやるのは避けるのが典型。
 * 今反転しているかどうかをフラグとして持っておけばよく、
 * T=2のクエリは反転状態を見て先頭or末尾どちらに入れればよいかが分かる。
 * 
 * 先頭に追加について、string型で s='c'+s とするのは遅そうな気がしたので、
 * 先頭末尾それぞれに追加できるdequeを使うこととした。
 */

int main(void)
{
	deque<char> q;
	string s; cin >> s;
	for(auto &e : s) q.push_back(e);
	int Q; cin >> Q;
	int rev = 0;
	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			rev = 1-rev;
		}
		else
		{
			int f; char c;
			cin >> f >> c;
			if( (f == 1 && rev == 0) || (f ==2 && rev == 1) )
			{
				q.push_front(c);
			}
			else
			{
				q.push_back(c);
			}
		}
	}
	if(rev == 1) reverse(q.begin(), q.end());
	for(auto &e : q) cout << e;
	cout << endl;

	return 0;
}
