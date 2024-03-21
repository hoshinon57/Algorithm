#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:62 https://atcoder.jp/contests/typical90/tasks/typical90_bj

/*
 * 入力例を元に試行錯誤してみたところ、N個全てを塗る必要があることから、
 * 最後は Ai=i or Bi=i というアイテムiが無いとダメっぽい。
 * これは有向グラフで考えたときに自己ループの辺になる。
 * 
 * 最後は自己ループの頂点になりそうなので、つまり最後から逆方向へ決まっていくことから、
 * ・X,Yからiへの有向辺を張る
 * ・自己ループの頂点からDFSして辿った頂点を記録していく
 *   (自己ループ頂点から1つたどれる頂点が、最後(自己ループ)の手前にて塗ることになる)
 * ・記録した頂点を反転したものが答
 * となる。
 * 
 * ※公式解説ではBFSでの実装だった。
 * 
 * [ACまでの思考の流れ]
 * ・操作をグラフで表してみる。[典型]
 * ・入力例に対し、どんな手順で決まっていくかを考えてみる。[マイ典型]
 */

using Graph = vector<vector<int>>;
Graph g;
vector<bool> seen;  // seen[i]:頂点iが探索済みならtrue
vector<int> li;

void dfs(int v)
{
	if(seen[v]) return;
	seen[v] = true;
	li.push_back(v);

	for(auto &e : g[v])
	{
		dfs(e);
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N; cin >> N;
	g.resize(N);
	seen.resize(N, false);
	vector<bool> self(N, false);  // 自己ループがあればtrue
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(i);
		g[b].push_back(i);
		if(a==i || b==i) self[i] = true;  // 自己ループ
	}

	// 自己ループの頂点からDFS
	for(i = 0; i < N; i++)
	{
		if(!self[i]) continue;
		dfs(i);
	}

	if((int)li.size() != N)
	{
		cout << -1 << endl;
		return 0;
	}
	reverse(li.begin(), li.end());
	for(auto &e : li)
	{
		cout << e+1 << endl;
	}

	return 0;
}
