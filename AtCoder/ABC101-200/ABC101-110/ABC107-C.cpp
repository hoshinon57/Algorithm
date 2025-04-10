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

// ABC107 https://atcoder.jp/contests/abc107

/*
 * 連続したK個のろうそくを使うのが良い。区間について全探索する。
 * 使うろうそくの左端をx[i]とすると、右端はx[i+K-1].
 * この区間について、まず左端に行ってから右へ進むケースと、右端に行ってから左へ進むケースがある。
 * これはそれぞれを計算して小さい方を採用する。
 */

int main(void)
{
	ll i;
	ll N, K; cin >> N >> K;
	vector<ll> x(N); for(i = 0; i < N; i++) {cin >> x[i];}

	ll ans = INF64;
	for(i = 0; i <= N-K; i++)  // 左側をi番目 そこからK個
	{
		ll xl = x[i];
		ll xr = x[i+K-1];
		ll tmp = (xr-xl) + min(abs(xl), abs(xr));  // min内: 原点(初期座標)から左端と右端のうち近い方を採用
		chmin(ans, tmp);
	}
	cout << ans << endl;

	return 0;
}
