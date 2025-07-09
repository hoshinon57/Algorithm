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

// ABC394 https://atcoder.jp/contests/abc394

/*
 * 難しい。コンテスト時に解けず、数か月後に再チャレンジしたときも複数回WAとなった。
 * 木DPにて解く。
 * 
 * 以下が参考になった。もしくはAC後のソース改善に参考になった。
 *   https://x.com/kyopro_friends/status/1893297918611800279
 *   https://atcoder.jp/contests/abc394/submissions/63021871
 *   https://atcoder.jp/contests/abc394/submissions/63019935
 * 
 * dp[v]を以下のように定義する。
 *   v以下の頂点について見ていて、
 *   (vを含めて)アルカンの部分木となるようなもののうち(※)、頂点数が最大のもの
 *   (※)により、親方向に1本辺を繋ぐ必要がある。
 * 
 * DFSで、vの子のうちdp[*]が大きい順から採用することとなる。
 * 子から3つ以上採用できたら、上位3つ+1(v自身) を持って根へ行くことになり、これがdp[v]となる。
 * 3つ以上採用できなかった場合は、自身のみを持って根へ行くので、dp[v]=1.
 * 
 * 一方で、vでアルカンが終端されるケースも考える。（これが中々思いつかなかった）
 * 子から1つ以上採用できたら、上位1つ+1 がアルカンの頂点数となり、
 * 4つ以上採用できたら、上位4つ+1 がアルカンの頂点数となる。
 * これはこの時点でansとchmaxしておく。
 * 
 * ここまでの処理では
 * ＞次数4の頂点が1つ以上存在する
 * という制約をケアできていない。(ans=1とか2になるケースがありそう)
 * アルカンなら頂点数は5以上なので、最後に ans<5 のチェックをすればよい。
**/

using Graph = vector<vector<int>>;

int main(void)
{
	ll i;
	ll N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		ll a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	ll ans = -1;
	vector<ll> dp(N);
	auto dfs = [&](auto &self, ll v, ll p = -1) -> void
	{
		ll cnt = 0;
		vector<ll> li;
		for(auto &e : g[v])
		{
			if(e == p) continue;
			self(self, e, v);
			cnt++;
			li.push_back(dp[e]);
		}
		sort(li.begin(), li.end(), greater<ll>());

		// 頂点vでアルカンが終端する場合
		if(cnt >= 1) chmax(ans, li[0]+1);  // +1は自身のぶん
		if(cnt >= 4) chmax(ans, li[0]+li[1]+li[2]+li[3]+1);

		// 根の方向へアルカンが伸びるケース
		// 3つ以上取れたら、上位3つを持って上へ
		if(cnt >= 3)
		{
			dp[v] = li[0]+li[1]+li[2]+1;
		}
		else dp[v] = 1;
	};
	dfs(dfs, 0);

	if(ans < 5) ans = -1;
	cout << ans << endl;

	return 0;
}
