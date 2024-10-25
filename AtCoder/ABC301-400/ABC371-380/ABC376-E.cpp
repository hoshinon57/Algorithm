#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC376 https://atcoder.jp/contests/abc376

/*
 * ソートすると処理が楽になる、という典型。
 * 
 * {A,B}をAで昇順ソートしておくと、A0から順に見ていきi番目のときに
 * max(A)=Aiとなるので、ΣBについてだけ考えればよくなる。
 * 
 * 優先度付きキューを使い、Bについて現在の集合の最大値を取り除くようにしていけば
 * 最小のK個の和を求めることができる。
 * 後は実装を参照のこと。
 */

void solve(void)
{
	int i;
	int N, K; cin >> N >> K;
	vector<pair<ll,ll>> d(N);  // {a,b}
	for(i = 0; i < N; i++) cin >> d[i].first;
	for(i = 0; i < N; i++) cin >> d[i].second;
	sort(d.begin(), d.end());

	ll ans = INF64;
	// 最初のK個は愚直
	priority_queue<ll> que;  // 大きい方が優先度高い
	ll bsum = 0;
	for(i = 0; i < K; i++)
	{
		bsum += d[i].second;
		que.push(d[i].second);
	}
	ans = d[K-1].first * bsum;

	// 残りN-K個
	for(i = K; i < N; i++)
	{
		// これまでのBの最大値を削除
		auto mxb = que.top();
		que.pop();
		bsum -= mxb;

		auto [a,b] = d[i];
		que.push(b);
		bsum += b;

		chmin(ans, a*bsum);
	}

	cout << ans << endl;
}

int main(void)
{
	int t; cin >> t;
	while(t > 0)
	{
		t--;
		solve();
	}

	return 0;
}
