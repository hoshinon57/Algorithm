#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC105 https://atcoder.jp/contests/abc105

/*
 * 各種解説と同じ方針。
 *   https://blog.hamayanhamayan.com/entry/2018/08/12/010816
 *   https://img.atcoder.jp/abc105/editorial.pdf
 * 
 * [l,r]のrについて全探索する。
 * Aの累積和を求めておき、mod Mが等しい箇所が条件を満たす区間となる。
 * 
 * M=2でAi=2のとき、[i,i]も条件を満たす。
 * これを簡単に処理できるよう、累積和の要素番号0から計算している。
 * 
 * [ACまでの思考の流れ]
 * ・ある区間[l,r]の和がMの倍数である。
 *   ⇒累積和の(l-1)番目とr番目をMで割った余りが等しい。 [典型]
 * ・区間[l,r]に関する問題は、rについて全探索し、条件を満たすlを高速に数え上げる。 [典型]
 * ・Mが最大10^9と、配列管理は難しい ⇒ mapを使う
 */

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> sum(N+1);
	for(i = 0; i < N; i++) sum[i+1] = sum[i] + a[i];

	ll ans = 0;
	map<ll,ll> cnt;
	for(i = 0; i <= N; i++)
	{
		ll d = sum[i]%M;
		ans += cnt[d];
		cnt[d]++;
	}
	cout << ans << endl;

	return 0;
}
