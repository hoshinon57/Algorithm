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

// ABC395 https://atcoder.jp/contests/abc395

/*
 * コンテスト時に解いた方針だが、微妙に未証明。
 * (ユーザー解説にある牛ゲーっぽい感じかなあ…違うかなあ…)
 * 
 * |ui-u(i+1)|<=Xの制約が厄介なので、まずこれを整理したい。
 * uは小さくできるが大きくはできない、という点に着目し、
 * ダイクストラ法の要領でuが小さい方から補正していく（確定させていく）。
 * 
 * するとu[i]+d[i]の最小値をHとすると、求める答になる。
**/

int main(void)
{
	ll i;
	ll N, X; cin >> N >> X;
	ll sum = 0;
	vector<ll> u(N), d(N); for(i = 0; i < N; i++) {cin >> u[i] >> d[i]; sum+=u[i]+d[i];}
	using pll = pair<ll,ll>;
	priority_queue<pll, vector<pll>, greater<pll>> que;  // {u,idx} 小さい方から
	for(i = 0; i < N; i++)
	{
		que.push({u[i], i});
	}

	// ダイクストラ法っぽく uが小さい方から確定していく
	while(!que.empty())
	{
		auto [pos,idx] = que.top();
		que.pop();

		if(pos > u[idx]) continue;  // 更新済み
		u[idx] = pos;
		if(idx > 0)
		{
			if(u[idx-1]-u[idx] > X)
			{
				que.push({u[idx]+X, idx-1});
			}
		}
		if(idx < N-1)
		{
			if(u[idx+1]-u[idx] > X)
			{
				que.push({u[idx]+X, idx+1});
			}
		}
	}

	ll mi = INF64;
	for(i = 0; i < N; i++)
	{
		chmin(mi, u[i]+d[i]);
	}
	// u+dをmiにそろえる
	ll ans = sum-mi*N;
	cout << ans << endl;

	return 0;
}
