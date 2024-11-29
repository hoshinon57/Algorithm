#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <deque>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_3_C

// 尺取り法でやる

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	while(Q > 0)
	{
		Q--;
		ll x; cin >> x;
		int l, r = 0;  // [l,r)
		ll sum = 0, ans = 0;
		for(l = 0; l < N; l++)
		{
			// rを1つ進めても大丈夫なら、進める
			// 半開区間なのでr=Nまで可能
			while(r < N && sum+a[r] <= x)
			{
				sum += a[r];
				r++;
			}
			ans += r-l;
			if(l==r) r++;
			else sum -= a[l];
		}
		cout << ans << endl;
	}

	return 0;
}
