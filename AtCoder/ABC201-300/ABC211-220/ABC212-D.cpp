#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC212 https://atcoder.jp/contests/abc212

/*
 * そのときの最小値を取り出す、は優先度付きキューで実現できる。
 * ただし操作2について、そのときのボール全てにXを加算するのはTLEとなる。
 * そこでオフセット値を持たせることを考える。
 * ・操作2：オフセット値を加算
 * ・操作1：(X-オフセット値)をキューに積む
 * ・操作3：(キューの先頭+オフセット値)を出力
 * とすればよい。
 * 
 * [ACまでの思考の流れ]
 * ・「全ての要素に加減算」は、別途オフセット値で管理する。
 *   (もしくはセグメント木か)
 * ・出力時にオフセット値を加算するならば、逆に入力時にオフセット値を減算してあげれば整合が取れそう。
 */

int main(void)
{
	int Q; cin >> Q;
	priority_queue<ll, vector<ll>, greater<ll>> que;
	ll offset = 0;
	while(Q > 0)
	{
		int k; cin >> k;
		if(k == 1)
		{
			ll x; cin >> x;
			que.push(x-offset);
		}
		else if(k == 2)
		{
			ll x; cin >> x;
			offset += x;
		}
		else
		{
			cout << que.top()+offset << endl;
			que.pop();
		}
		Q--;
	}

	return 0;
}
