#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC225 https://atcoder.jp/contests/abc225

/*
 * ※公式解説と考え方、実装方針が同一であった。
 * 
 * 各電車xについて、xの前にある電車の番号をprev[x]で、後側をnext[x]で表す。
 * クエリ1,2はそのまま実装すればよい。
 * クエリ3も愚直に「まずはprevを用いて先頭まで行き、そこから最後尾までnextを用いて表示していく」とすればよい。
 * 制約より出力の個数は10^6以下であることから、prev/nextで辿る数もそれと同等となるため、愚直でもTLEはしない。
 */
int main(void)
{
	int N, Q; cin >> N >> Q;
	vector<int> prev(N+1, -1), next(N+1, -1);  // prev[x]:電車xの前にある電車の番号  無い場合は-1

	while(Q > 0)
	{
		int q; cin >> q;
		if(q == 1)
		{
			int x, y; cin >> x >> y;
			next[x] = y;
			prev[y] = x;
		}
		else if(q == 2)
		{
			int x, y; cin >> x >> y;
			next[x] = -1;
			prev[y] = -1;
		}
		else
		{
			int x; cin >> x;
			// まず先頭へ
			while(prev[x] != -1)
			{
				x = prev[x];
			}
			// 先頭から順に出力
			vector<int> ans;
			do
			{
				ans.push_back(x);
				x = next[x];
			} while (x != -1);
			cout << ans.size() << " ";
			for(int i = 0; i < (int)ans.size(); i++)
			{
				cout << ans[i];
				if(i != (int)ans.size()-1) cout << " ";
			}
			cout << endl;
		}
		Q--;
	}

	return 0;
}
