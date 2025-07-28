#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC416 https://atcoder.jp/contests/abc416

/*
 * コンテスト時はmultisetにてAを管理し、マッチングさせていった。
 *   https://atcoder.jp/contests/abc416/submissions/67934797
 * 
 * 以下は公式解説を元に実装した版。
 *   https://atcoder.jp/contests/abc416/editorial/13487
 *   https://x.com/kyopro_friends/status/1949103672568537591
 * 
 * 基本は公式解説の通り。
 * (a+b)%Mは、a+b<Mならa+b, a+b>=Mならa+b-Mとみなせる。「%Mは足してM以上になったらMを引く」という典型。
 * Σ((a+b)%M)は、a+b>=Mとマッチングできる個数をCとすると、Σ(a+b)-C*M と置き換えられるので、Cの最大化を考える。
 * これは各Aiについて(M-Ai)以上の最小のBとマッチングさせる形になり、
 * Aを降順ソート、Bを昇順ソートしておくと尺取り法の要領で解ける。
 * 詳細は実装を参照。
**/

void solve(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	sort(a.begin(), a.end(), greater<ll>());
	sort(b.begin(), b.end());
	ll ai = 0, bi = 0;
	ll c = 0;
	for(ai = 0; ai < N; ai++)
	{
		while(a[ai]+b[bi] < M && bi < N) bi++;  // ai+bi>=Mになるまで
		if(bi == N) break;
		// a[ai]とb[bi]がマッチ
		c++;
		bi++;
	}
	ll ans = -c*M;
	for(i = 0; i < N; i++)
	{
		ans += a[i]+b[i];
	}
	cout << ans << endl;
}

int main(void)
{
	ll T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}
	return 0;
}
