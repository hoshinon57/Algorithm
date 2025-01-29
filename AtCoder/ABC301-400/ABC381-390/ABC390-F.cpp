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

// ABC390 https://atcoder.jp/contests/abc390

/*
 * 自力で解けず、解説を見た。
 * 包除原理や「寄与に着目」の主客転倒などで解く。いやーこれ難しい…
 * 基本は各種解説の通り。
 *   https://atcoder.jp/contests/abc390/editorial/11968
 *   https://x.com/kyopro_friends/status/1883153958992294051
 *   https://atcoder.jp/contests/abc390/submissions/62107853  snuke氏のコード
 * 
 * 操作(l,r)にてl=cが存在するf(L,R)の個数を数え上げることを考える。
 * これをg(c)とすると、Σ(i:1～N)g(i) が答。
 * 
 * 集合{AL～AR}にてl=cとなる操作が存在することは、
 *   集合にAi=cを含む
 *   集合にAi=(c-1)を含まない
 * の両方を満たすことが条件。
 * 後述するが、「xを含まない」の数え上げは簡単であることを踏まえ、包除原理にて
 *   「(c-1)を含まない」-「cも(c-1)も含まない」
 * がg(c)となる。
 * 
 * 「xを含まない」の数え上げは、Ai=xとなる登場位置をidx[x]に記録していくと、
 * df = idx[x][i]-idx[x][i-1]-1 として df*(df+1)/2 の総和となる。
 * (実装では先頭と末尾に番兵を入れ、ソートしている)
 * これがg(x)になる。
 */

int main(void)
{
	// 1-indexed
	ll i;
	ll N; cin >> N;
	vector<vector<ll>> idx(N+1);	// idx[x]:a[i]=xの登場位置 1-indexed
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		idx[a[i]].push_back(i+1);
	}

	// 登場位置を保持するidx[]にて、それを含まない区間の総数を返す
	auto func = [&](vector<ll> _idx) -> ll
	{
		// 番兵
		_idx.push_back(0);
		_idx.push_back(N+1);
		sort(_idx.begin(), _idx.end());

		ll ret = 0;
		int sz = (int)_idx.size();
		for(int j = 1; j < sz; j++)
		{
			ll df = _idx[j] - _idx[j-1] - 1;
			ret += df*(df+1)/2;
		}

		return ret;
	};

	ll ans = 0;
	for(i = 1; i <= N; i++)  // 操作(l,r)にてl=iが存在するような区間(L,R)の個数を求めていく
	{
		auto mix = idx[i];
		for(auto &e : idx[i-1]) mix.push_back(e);
		ll tmp1 = func(mix);
		ll tmp2 = func(idx[i-1]);
		ans += (tmp2-tmp1);
	}
	cout << ans << endl;
	
	return 0;
}
