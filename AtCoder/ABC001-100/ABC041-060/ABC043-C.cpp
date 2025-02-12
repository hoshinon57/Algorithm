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

// ABC043 https://atcoder.jp/contests/abc043

// 「どの値に揃えるか？」を全探索。
// 要素a[]の中央値が答になる？とか考えたが、各種解説を見てもそういった言及はなかったので、違いそう。

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = INF64;
	for(i = -100; i <= 100; i++)  // iに揃える
	{
		ll sum = 0;
		for(auto &e : a)
		{
			sum += (i-e)*(i-e);
		}
		chmin(ans, sum);
	}
	cout << ans << endl;

	return 0;
}
