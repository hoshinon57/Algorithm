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

// ABC397 https://atcoder.jp/contests/abc397

/*
 * 考察間違いによりコンテスト時に解けず。うおー悔しい。
 * 
 * 木をどうパスに分解していくかを考えると、葉から順に切っていくしか無いかな、となる。
 * よって木DPで以下の情報を管理していく。
 *   dp[v]:頂点vを含む部分木について、パスへ未分解である頂点の個数
 * dp[v]>=Kになったらvを含むパスに分解できる…となるが、色々と条件がある。
 * 
 * 各vにて、子頂点eについてdp[e]!=0である個数をcとする。
 * するとcによって分解できるかが変わってくる。
 * 1)c>=3の場合
 *   「頂点vを通るパス」を作りたいが、どうやってもパスにはできない。
 *   Noで終了。
 *   ※ここ、コンテスト時に考察ミスった。パスを作れるとバカなことを考えてしまっていた…
 * 2)c=2の場合
 *   パスを作れて、かつvの親の方向にも矛盾なく渡せるのは、dp[v]=Kのときのみ。
 *   それ以外はNoで終了。
 *   ※木の根のときもこれで良いかな…となるが、
 *     頂点数がNKなので、dp[0]は必ず0になり、問題なさげ。
 * 3)c=0,1の場合
 *   特に矛盾は起きない。
 *   dp[v]=Kになったらパスを作る。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・考察の方向性は合っていたが、"パス"なので上記のc>=3でも処理を続けられるとかバカな方向に進んでしまった。
 *   手でサンプルなどグラフを描いた結果がこうだったし(しかもc>=3のケースをたまたま描いていた)、どうやったらミスを防げるか…
**/

using Graph = vector<vector<int>>;
vector<int> dp;  // dp[v]:頂点vを含む部分木について、パスへ未分解である頂点の個数
int N, K;

void no(void)
{
	cout << "No" << endl;
	exit(0);
}

void dfs_tree_dp(Graph &g, int v, int p = -1)
{
	int c = 0;  // vの子頂点eについて、dp[e]!=0である個数
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs_tree_dp(g, e, v);  // まずは子へ再帰してしまう
		// 子のDP値が確定しているので、親側にて自由に使う
		dp[v] += dp[e];
		if(dp[e] != 0) c++;
	}
	dp[v]++;  // 自身の分
	if(c >= 3)  // 3以上の場合、パスには分解できない
	{
		no();
	}
	else if(c == 2)  // 2の場合、v以下の部分木で余りなくパスを作れないと、全体としてNG
	{
		if(dp[v] != K) no();
		else dp[v] = 0;  // ちょうど頂点使い切った
	}
	else  // c=0,1
	{
		if(dp[v] >= K) dp[v] -= K;  // ロジック的には if(dp[v]==K) dp[v]=0; と等価
	}
}

int main(void)
{
	int i;
	cin >> N >> K;
	int NK = N*K;
	Graph g(NK);
	for(i = 0; i < NK-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dp.resize(NK);
	dfs_tree_dp(g, 0);
	cout << "Yes" << endl;

	return 0;
}
