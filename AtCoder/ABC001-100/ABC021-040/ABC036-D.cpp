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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC036 https://atcoder.jp/contests/abc036

/*
 * 木DPにて解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   頂点iを根とした部分木について、
 *   頂点iの色がj=0:白, 1:黒 であるときの
 *   塗り方の総数
 * 
 * dp[i][0]は、子が白黒どちらでもよいので、(dp[e][0]+dp[e][1])の積となる。
 * dp[i][1]は、子が白でないとダメなので、dp[e][0]の積となる。
 * 初期値は1.
**/

using Graph = vector<vector<int>>;
vector<vector<mint>> dp;

void dfs_tree(Graph &g, int v, int p = -1)
{
	dp[v][0] = dp[v][1] = 1;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs_tree(g, e, v);
		dp[v][0] *= dp[e][0]+dp[e][1];
		dp[v][1] *= dp[e][0];
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	dp.resize(N, vector<mint>(2));
	dfs_tree(g, 0);
	cout << (dp[0][0]+dp[0][1]).val() << endl;

	return 0;
}
