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

// ABC115 https://atcoder.jp/contests/abc115

/*
 * メモ化再帰で解く。メモのキーは{L,食べる枚数}とする。
 * メモ化しないとNが大きいケースで間に合わない。
 * 各レベルから「下側のL-1を食べ切り」「上側のL-1にも着手する」ケースで2回再帰するため。
 * 各種解説では各レベルの肉の枚数を事前計算しておき、「下側のL-1を食べ切り」を再帰せずO(1)で計算していた。
 * この場合メモ化は不要。なるほど。
**/

vector<ll> tot;  // tot[i]:レベルiの総枚数
map<pair<ll,ll>, ll> memo;  // {レベルlevバーガー,下からnum枚} の肉の数

// レベルlevバーガーの下からnum枚の肉の数を返す
// tot[lev] >= numであること
ll func(ll lev, ll num)
{
	if(lev == 0) return 1;
	if(memo.count({lev,num}) > 0) return memo[{lev,num}];

	ll &ret = memo[{lev,num}];
	ret = 0;

	// パン
	if(--num <= 0) return ret;

	// lev-1
	ret += func(lev-1, min(num, tot[lev-1]));
	num -= tot[lev-1];
	if(num <= 0) return ret;

	// 肉
	ret++;
	if(--num <= 0) return ret;

	// lev-1
	ret += func(lev-1, min(num, tot[lev-1]));

	return ret;
}

int main(void)
{
	ll i;
	ll N, X; cin >> N >> X;
	tot.resize(N+1);
	tot[0] = 1;
	for(i = 1; i <= N; i++)
	{
		tot[i] = tot[i-1]*2 + 3;
	}
	cout << func(N, X) << endl;

	return 0;
}
