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

// ABC402 https://atcoder.jp/contests/abc402

/*
 * 公式解説と実装同じだった。
 *   https://atcoder.jp/contests/abc402/editorial/12757
 * 
 * 2直線の組み合わせは C(M,2).
 * 直線が交わらない、つまり平行をカウントしたのを引く。
 * (A+B)%N が同じなら平行となるので、これでカウントする。
 * 
 * [ACまでの思考の流れ]
 * ・数え上げで、余事象で考えてみる。[典型]
 */

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> cnt(N);
	for(i = 0; i < M; i++)
	{
		ll a, b; cin >> a >> b;
		cnt[(a+b)%N]++;
	}

	ll ans = M*(M-1)/2;
	for(auto &e : cnt)
	{
		ans -= e*(e-1)/2;
	}
	cout << ans << endl;

	return 0;
}
