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

// ABC389 https://atcoder.jp/contests/abc389

// ヘビを実際に取り除かず、オフセットで管理するやつ。

int main(void)
{
	vector<ll> top;  // top[x]:x番目のヘビの先頭座標
	ll nxt = 0;  // 次にヘビが追加されるときの、座標
	int ofs = 0;
	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int kind; cin >> kind;
		if(kind == 1)
		{
			ll l; cin >> l;
			top.push_back(nxt);
			nxt += l;
		}
		else if(kind == 2)
		{
			ofs++;
		}
		else
		{
			int k; cin >> k;
			k--;
			cout << top[k+ofs] - top[ofs] << endl;
		}
	}

	return 0;
}
