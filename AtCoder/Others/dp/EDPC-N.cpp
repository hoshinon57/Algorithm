#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 区間DPで解く。
 * 
 * dp[l][r]を以下のように定義する。
 *   区間[l,r)のスライムを1匹にまでするための、コスト総和の最小値  ※半開区間
 * dp[0][N]が答。
 * 
 * [l,r)が1匹になるとき、直前の状態は
 *   [l,l+1) と [l+1,r) を合体
 *   [l,l+2) と [l+2,r) を合体
 *   …
 *   [l,r-1) と [r-1,r) を合体
 * のどれかとなり、これらの最小値を採用することになる。
 * つまりそれぞれの区切りについて全探索する。
 * ※どの区切りについても、問題文にある「太郎君は x+y のコストを支払う」は同一値となる。
 *   よって、解説では事前に累積和を求めているが、愚直にやっても十分に間に合った。
 * 
 * 実装はメモ化再帰とした。
 * ベースケースは l+1=r つまりスライムが1匹のときで、この場合は合体をしないのでコストは0.
 */

int N;
vector<ll> a;
vector<vector<ll>> dp;

ll func(int l, int r)
{
	ll &d = dp[l][r];
	if(d != -1) return d;
	if(r-l == 1) return 0;  // ベースケース 1匹なら合体はしない

	int i;
	ll sum = 0;
	for(i = l; i < r; i++) sum += a[i];  // [l,r)を合体するためのコスト 事前に累積和を求めなくても十分に間に合った
	d = INF64;
	for(i = l+1; i < r; i++)  // [l,i) と [i,r) に分ける
	{
		ll tmp = func(l, i) + func(i, r);
		chmin(d, tmp);
	}
	d += sum;

	return d;
}
int main(void)
{
	int i;
	cin >> N;
	a.resize(N);
	for(i = 0; i < N; i++) cin >> a[i];
	dp.resize(N+1, vector<ll>(N+1, -1));

	cout << func(0, N) << endl;

	return 0;
}
