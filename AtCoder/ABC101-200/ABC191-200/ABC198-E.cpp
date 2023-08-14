#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC198 https://atcoder.jp/contests/abc198

/*
 * 深さ優先探索で解く。
 * 
 * 根である頂点1からDFSしていき、
 *   c_cnt[i]:頂点1から今いる頂点までに、色iである頂点の数
 * を行きがけ時に+1, 帰りがけ時に-1していく。
 * 行きがけ時にこれが0であれば、よい頂点となる。
 * 
 * 木なので頂点数N, 辺数N-1であるため、計算量はO(N).
 */

using Graph = vector<vector<int>>;
vector<int> c;  // c[N]
vector<int> c_cnt;  // c_cnt[max C]
vector<int> ans;

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// p:vの親
void dfs_tree(Graph &g, int v, int p = -1)
{
	int col = c[v];  // 今見ている頂点vの色
	if(c_cnt[col] == 0)  // 頂点vの色が未使用なら、よい頂点
	{
		ans.push_back(v);
	}
	c_cnt[col]++;

	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		dfs_tree(g, e, v);
	}
	c_cnt[col]--;
}

int main(void)
{
	int i;
	int N; cin >> N;
	c.resize(N);
	for(i = 0; i < N; i++) {cin >> c[i];}
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	c_cnt.resize(100005);
	dfs_tree(g, 0);
	sort(ans.begin(), ans.end());
	for(auto &e : ans)
	{
		cout << e+1 << endl;
	}

	return 0;
}
