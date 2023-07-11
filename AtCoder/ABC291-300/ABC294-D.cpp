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

// ABC294 https://atcoder.jp/contests/abc294

/*
 * 「受付にまだ呼ばれていない人のうち、最も小さい番号」を数値で管理し、
 * 「受付に呼ばれたが、まだ行っていない人」を集合で管理する。
 */

int main(void)
{
	int N, Q; cin >> N >> Q;
	int next = 1;  // 受付にまだ呼ばれていない人のうち、最も小さい番号
	set<int> called;  // 受付に呼ばれたが、まだ行っていない人の集合

	while(Q > 0)
	{
		int kind; cin >> kind;
		int x;
		if(kind == 1)
		{
			called.insert(next);
			next++;
		}
		else if(kind == 2)
		{
			cin >> x;
			called.erase(x);
		}
		else
		{
			cout << *called.begin() << endl;
		}

		Q--;
	}

	return 0;
}
