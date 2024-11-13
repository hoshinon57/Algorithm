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

// ABC379 https://atcoder.jp/contests/abc379

/*
 * コンテスト時に考えた方針。右のマスから埋めていく。
 * 以下、1-indexedで記載する。概要は以下。
 * ・次に石を置くべきマスのうち、最右をnxとする。
 * ・{X,A}をXでソートし、大きい方から見ていく。
 *   マスXiにある石Ai個を全て右へ動かしても「2個以上になるマス」が存在したらNG.
 *   XiについてOKなら、マスXiにあるAi個の石を [nx-Ai+1,nx] の範囲に配る。
 *   これは等差数列でO(1)で求められる。
 *   nxをnx-aで更新する。
 * ・最後まで見たときにnx=!0ならNG.
 *   これはΣA!=Nのケースに相当するか。
 */

int main(void)
{
	// 1-indexed
	ll i;
	ll N, M; cin >> N >> M;
	using pll = pair<ll,ll>;
	vector<pll> st(M);  // {X,A}
	for(i = 0; i < M; i++) cin >> st[i].first;
	for(i = 0; i < M; i++) cin >> st[i].second;
	sort(st.begin(), st.end());

	ll ans = 0;
	ll nx = N;  // 次に置くべきマス(最右)
	for(i = M-1; i >= 0; i--)
	{
		auto [x,a] = st[i];
		if(nx-x+1 < a)  // マスxの石a個を全て右に配っても「2個以上になるマス」が存在したらNG
		{
			cout << -1 << endl;
			return 0;
		}

		// マスxにあるa個の石を[nx-a+1,nx]に配る
		ll start = (nx-a+1)-x;
		ll end = nx-x;
		ans += (start+end)*a/2;  // 等差数列 項数a
		nx -= a;
	}
	if(nx != 0)  // 左端のマスに石が置かれていない NG
	{
		cout << -1 << endl;
		return 0;
	}
	cout << ans << endl;

	return 0;
}
