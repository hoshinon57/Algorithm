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

// ABC385 https://atcoder.jp/contests/abc385

/*
 * コンテスト時、調和級数の考え方で解いた。
 * 左端のビルiと間隔jを決め打ちし、条件を満たすように何棟のビルを選べるかを求める。これの最大値が答。
 * 間隔jを1～Nまで変えていったとき、それぞれチェックするビルは
 *   0,1,2,...N  (N/1)個
 *   0,2,4,...N  (N/2)個
 *   0,3,6,...N  (N/3)個
 * となり、調和級数によりjについてO(NlogN)となる。
 * よって左端のビルiについて全探索してもO(N^2logN)となり、間に合う。
 * 
 * また公式解説のO(N^2)解法でも実装してみた。#ifの#else側。
 * ビルの間隔kについて全探索する方針。
 * O(N^2logN)が32msに対してこちらは11msと、確かに早かった。
 */

int main(void)
{
	ll i, j, k;
	ll N; cin >> N;
	vector<ll> h(N); for(i = 0; i < N; i++) {cin >> h[i];}
	ll ans = 0;
#if 1
	for(i = 0; i < N; i++)  // 左端のビルi
	{
		for(j = 1; j <= N; j++)  // 間隔j (j<Nだとビルが1つのときに正しく計算できない)
		{
			ll cnt = 0;
			for(k = i; k < N; k+=j)
			{
				if(h[i] == h[k]) cnt++;
				else break;
			}
			chmax(ans, cnt);
		}
	}
#else
	for(k = 1; k <= N; k++)  // 間隔k (k<Nだとビルが1つのときに正しく計算できない)
	{
		for(i = 0; i < k; i++)  // グループi
		{
			ll cnt = 1;
			for(j = i; j < N; j+=k)
			{
				if(j==i || h[j] != h[j-k])  // 初回 or 前のビルと異なる
				{
					cnt = 1;
				}
				else  // 前のビルと同じ高さ
				{
					cnt++;
				}
				chmax(ans, cnt);  // cntがリセットされることもあるので、毎回更新必要
			}
		}
	}
#endif
	cout << ans << endl;

	return 0;
}
