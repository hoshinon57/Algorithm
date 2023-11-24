#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC217 https://atcoder.jp/contests/abc217

/*
 * 公式解説の通り。
 *   https://atcoder.jp/contests/abc217/editorial/2578
 *   https://kanpurin.hatenablog.com/entry/2021/09/05/163703
 * 
 * 木材の切れ目の座標をsetで管理し、
 * c=2のクエリはxより大きい切れ目と小さい切れ目の差を出力すればよい。
 * 木材の両端である0,Lに番兵を入れておくと処理が楽。
 */

int main(void)
{
	int L, Q; cin >> L >> Q;
	set<int> w;
	w.insert(0); w.insert(L);  // 番兵
	while(Q > 0)
	{
		int c, x; cin >> c >> x;
		if(c == 1)
		{
			w.insert(x);
		}
		else
		{
			int l, r;
			auto itr = w.upper_bound(x);
			r = *itr;
			itr--;
			l = *itr;
			cout << r-l << endl;
		}
		Q--;
	}

	return 0;
}
