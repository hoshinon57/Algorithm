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

// ABC125 https://atcoder.jp/contests/abc125

// https://blog.hamayanhamayan.com/entry/2019/04/28/091246
// と全く同じ考え方だった
//
// 負の数が偶数ならAを全て正にできる。
// 奇数なら1つだけ負、残りは正にできる。この場合、絶対値が最小のものを負にすればよい。

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N);
	int num = 0;  // a[]<0である要素数
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		if(a[i] < 0) num++;
	}

	ll ans = 0;
	for(i = 0; i < N; i++) ans += abs(a[i]);
	if(num%2 == 1)
	{
		sort(a.begin(), a.end());
		ll m = INF64;
		for(i = 0; i < N; i++)
		{
			chmin(m, abs(a[i]));
		}
		ans -= m*2;  // 先ほど足したのを取りやめて、かつ引く
	}
	cout << ans << endl;

	return 0;
}
