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

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 全方位木DPで解く。
 * ※方向性は思い浮かんだものの、"累積積"の部分が思いつかず、各種解説を見た。
 *   https://blog.hamayanhamayan.com/entry/2019/01/12/152428
 *   https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106
 *   https://kyopro.hateblo.jp/entry/2019/01/29/173135
 * 
 * dp[v]を以下のように定義する。
 *   決め打ちである頂点を根としたときに、
 *   頂点vの部分木について、頂点vが黒、もしくはv以下が全て白で
 *   制約をみたすような組み合わせの数
 * 
 * まず、決め打ちで頂点0を根としたときに、dp[v]を求める。
 * これはvの子をeとしたときに、 dp[e1]*dp[e2]*...*dp[en] + 1 となる。(+1は頂点v以下がすべて白のケース)
 * 結果、通常の木DPで求められる。
 * ⇒dfs_1()を参照。
 * 
 * 頂点vの答をans[v]としたとき、ans[0]=dp[0].
 * では他の頂点はどうするか。
 * 頂点v->eへ遷移するとき、eにとってのdp[v]は
 *   dp[v]の計算式から、dp[e]を取り除いたもの
 * と考えることができる。
 * よってdp[v]を再計算してからeへ遷移すれば、前述の式でdp[e]およびans[e]を求められる。
 * 
 * ただしこの計算を dp[v] = (dp[v]-1) / dp[e] のようにするのはNG.
 * 理由はMが素数とは限らないことから、dp[e]の逆元が存在しないケースがあるため。
 * 除算を使わずに求める必要がある。
 * 
 * ここで累積和ならぬ累積積を使う。
 * 頂点vとその子e1,e2,...について、
 *   left[i]:左からi個のdp[e]の積
 *   right[i]:右から同様
 * とする。
 * 頂点enへ遷移するとき、上記を用いることで
 * dp[v]をdp[en]を除いた積にO(1)で置き換えることができるので、遷移していけばよい。
 * [典型]左右から累積和
 */

ll N, M;
vector<ll> dp, ans;
using Graph = vector<vector<int>>;

ll dfs_1(Graph &g, int v, int p = -1)
{
	dp[v] = 1;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dp[v] *= dfs_1(g, e, v);
		dp[v] %= M;
	}
	dp[v]++;  // 全頂点が白のケース
	return dp[v];
}

void dfs_2(Graph &g, int v, int p = -1)
{
	ll i;
	ll sz = g[v].size();
	vector<ll> left(sz+1, 1), right(sz+1, 1);  // 累積和ならぬ累積積
	for(i = 0; i < sz; i++)
	{
		left[i+1] = (left[i] * dp[g[v][i]]) % M;
		right[i+1] = (right[i] * dp[g[v][sz-i-1]]) % M;
	}

	ans[v] = left[sz];  // 頂点vが黒のときの組み合わせ数、なので+1は不要

	for(i = 0; i < sz; i++)
	{
		auto e = g[v][i];
		if(e == p) continue;
		dp[v] = (left[i] * right[sz-i-1] + 1) % M;  // dp[e]を除いた積に変更
		dfs_2(g, e, v);
	}
}

int main(void)
{
	int i;
	cin >> N >> M;
	Graph g(N);
	dp.resize(N);
	ans.resize(N);
	for(i = 0; i < N-1; i++)
	{
		ll x, y; cin >> x >> y;
		x--; y--;
		g[x].push_back(y);
		g[y].push_back(x);
	}
	dfs_1(g, 0);
	dfs_2(g, 0);

	for(auto &e : ans) cout << e << endl;

	return 0;
}
