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

// ABC288 https://atcoder.jp/contests/abc288

/*
 * コンテスト時に解けず、解説を見た。
 * 操作による不変量に着目して解く。
 * 
 * 連続したK個に加算する、を別の見方をすると、
 *   要素番号をmod Kでグループ分けしたときに、 (例:K=3にて{0,3,6},{1,4,7},{2,5,8})
 *   グループ内での総和をSkとすると、S0,S1,S2,...の差は操作によって変わらない。
 * という事実が分かる。
 * よって、操作によってSkは等しく増加減少することから、全てのSkが同一ならば良い数列である。
 * (厳密には十分条件とか必要条件とかあるが)
 * 
 * 上記を判定するために、少し特殊な、以下の累積和を考える。
 *   sum[i+1][j]: Aの先頭i個の累積和で、i%K=jであるもののみ加算したもの
 * これにより、クエリを半開区間[l,r)にしたときに
 * sum[r][*]-sum[l][*]が全ての"*"について一致していればOKである。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・操作の問題では、操作による不変量に着目する。
 *   本問ではK個ごとにグループ分けしたときに、操作によってグループ内総和の差は変化しない、のが不変量となる。
 */

int main(void)
{
	ll i, j;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<vector<ll>> sum(N+1, vector<ll>(K, 0));  // sum[i+1][j] Aの先頭i個の累積和で、i%K=jであるもののみ加算したもの
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < K; j++)  // sum[i][j]
		{
			sum[i+1][j] = sum[i][j];
			if(j == i%K) sum[i+1][j] += a[i];
		}
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll l, r; cin >> l >> r;
		l--; r--;  // 0-indexedへ変換
		r++;  // [l,r)

		// sum[r][*]-sum[l][*] が全て一致していればOK
		bool ok = true;
		for(j = 1; j < K; j++)
		{
			ll tmp1 = sum[r][j] - sum[l][j];
			ll tmp2 = sum[r][j-1] - sum[l][j-1];
			if(tmp1 != tmp2) ok = false;
		}
		YesNo(ok);
	}

	return 0;
}
