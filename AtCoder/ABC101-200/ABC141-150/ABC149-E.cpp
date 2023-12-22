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

// ABC149 https://atcoder.jp/contests/abc149

/*
 * 二部探索で解く。
 * 
 * 問題文の意味は結局、以下のイメージ。
 *   横軸にA1～AN
 *   縦軸にA1～AN
 *   となる表があり、各マスは縦横軸の和になっている。
 *   このとき大きい方からM個の総和を求めよ。
 * Mが最大で10^10なので、愚直にやっても間に合わない。
 * 
 * ここで「Ax+AyがX以上になる組み合わせが、M個以上あるか？」という判定問題を考える。
 * Xを大きくしていくと true,true,...,true,false,false,... とどこかで切り替わる。
 * このT/Fの 境界部分 に答があるので、二部探索で求める。
 * 
 * 判定問題は、Aをソートしておいて、
 * 各Aiに対してX-Ai以上となるAの要素数をlower_bound()で求めて加算していけばよい。
 * 実装ではこの個数を返している。
 * 
 * 本問はもうちょい処理が必要で、
 * 二分探索で求めたr以上の総和をまず求める。これは累積和を用いた。
 * また和がlになる個数も(M-R)個必要なので、l*(M-R)を加算して完了。
 * 
 * [ACまでの思考の流れ]
 * ・問題文の意味を理解すると、A*Aの表のうち大きい方からM個取るということ。
 * ・愚直にM個はTLEなのでどうするか。 ⇒ここで二分探索を思いついた。
 *   「大きい方からM個」を「あるXより大きいのはM個以上？ M個未満？」という判定で境界を探すイメージ。
 */

int main(void)
{
	int i;
	ll N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end());

	ll l = 0;
	ll r = INF32;
	// Ax+AyがX以上になるケースのみ採用したときに、それを満たす個数を返す
	auto calc = [&](ll x) -> ll
	{
		ll cnt = 0;
		for(auto &e : a)
		{
			cnt += a.end() - lower_bound(a.begin(), a.end(), x-e);
		}
		return cnt;
	};
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(calc(mid) >= M) l = mid;
		else r = mid;
	}

	ll ans = 0;
	vector<ll> a_sum(N+1);
	for(i = 0; i < N; i++) a_sum[i+1] = a_sum[i] + a[i];
	// Ax+Ayがr以上になるものについて、その総和を求める
	for(auto &e : a)
	{
		int idx = lower_bound(a.begin(), a.end(), r-e) - a.begin();
		ans += a_sum[N] - a_sum[idx];  // Axを固定したときのAy部分
		ans += e * (N-idx);  // Ax部分
	}
	ll R = calc(r);
	ans += l*(M-R);  // Ax+Ay=lの要素を足すことで、全部でM個になる
	cout << ans << endl;
	return 0;
}
