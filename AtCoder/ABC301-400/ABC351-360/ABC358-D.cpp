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

// ABC358 https://atcoder.jp/contests/abc358

/*
 * 尺取り法で解く。コンテスト時は実装にミスり2WAもしてしまった…
 * A,Bともにソートしておき、各BについてA>=Bとなる最小のAを選んでいけばよい。
 */

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(M); for(i = 0; i < M; i++) {cin >> b[i];}
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	int ai = 0;
	ll ans = 0;
	for(i = 0; i < M; i++)  // b[i]
	{
		// 条件を満たすまでA[]のidxを移動
		while(ai<N && a[ai]<b[i]) ai++;

		if(ai == N)
		{
			ans = -1;
			break;
		}
		ans += a[ai];
		ai++;
	}
	cout << ans << endl;

	return 0;
}
