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

// ABC227 https://atcoder.jp/contests/abc227

/*
 * 自力で解けず、解説を見た。
 * 二分探索で解く。
 * 
 * 「(K人からなる)プロジェクトをX個作れるか？」の判定問題を考えると、
 * Xを増やしていくにつれ、 true,true,...,true,false,false,... とどこかでT/Fが切り替わる。
 * これを二分探索で求める。
 * 
 * 判定問題は以下のように解く。
 * ・各a[i]について、min(a[i],X)をsumに加算していく。
 * ・sum>=K*Xであればtrue.
 * min()は、プロジェクトをX=5個作りたい場合、A=10人の部署があっても5人までしか参画できないため。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・作れるプロジェクト数で考えると、どこかで 作れる/作れない が切り替わりそう。
 *   ⇒二分探索で解けないか。
 * ・貪欲に「Aの降順に、いま最も人数が少ないプロジェクトに人を追加していく」というのを最初考えたが、
 *   N=5,K=2, A={4,3,3,2,2} で答=6となりNG.(正解は7)
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	// K人からなるプロジェクトをx個作れるか
	auto check = [&](ll x) -> bool
	{
		ll sum = 0;
		for(i = 0; i < N; i++)
		{
			sum += min(a[i], x);
		}
		// return (sum >= K*x);  // これだとオーバーフローしてしまう
		return (x <= sum/K);
	};
	ll l = 0, r = INF64;
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}
	cout << l << endl;

	return 0;
}
