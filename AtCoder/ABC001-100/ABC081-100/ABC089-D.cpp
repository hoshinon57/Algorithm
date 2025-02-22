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

// ABC089 https://atcoder.jp/contests/abc089

/*
 * 累積和を用いて解く。
 * クエリ全体でDが固定されているので、例えばD=3であれば、1を基点に1,4,7,10,... の距離を求めることができる。
**/

int main(void)
{
	ll i, j;
	ll H, W, D; cin >> H >> W >> D;
	ll N = H*W;
	vector<pair<ll,ll>> pos(N);  // pos[n] = {y,x}
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			int a; cin >> a;
			a--;
			pos[a] = {i, j};
		}
	}
	auto di = [&](int p, int q) -> ll
	{
		ll tmp1 = abs(pos[p].first - pos[q].first);
		ll tmp2 = abs(pos[p].second - pos[q].second);
		return tmp1+tmp2;
	};
	vector<ll> sum(N);
	for(i = 0; i < N; i++)
	{
		if(i-D >= 0) sum[i] = sum[i-D] + di(i-D, i);
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int l, r; cin >> l >> r;
		l--; r--;
		cout << sum[r]-sum[l] << endl;
	}

	return 0;
}
