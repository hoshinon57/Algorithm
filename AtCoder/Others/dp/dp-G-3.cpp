#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

// トポロジカルソートのライブラリに最長経路を追加したので、それで解いた版。

using Graph = vector<vector<int>>;

// サイクルの無い有向グラフGについて、トポロジカルソートを行う。
// ソートした結果の頂点をresultに、最長経路(=辺の本数)をdepthに、それぞれ格納する。
// resultとdepthはそれぞれ空要素で渡してよい。
// ※トポロジカルソートできない場合(サイクルがある場合)、G.size()とresult.size()が不一致かどうかで判断可能。
// 参考：
//   https://algo-logic.info/topological-sort/
//   https://qiita.com/Morifolium/items/6c8f0a188af2f9620db2
void bfs_topological_sort(Graph &G, vector<int> &result, vector<int> &depth)
{
	int i;
	int n = G.size();

	vector<int> indegree(n);  // 入次数
	for(i = 0; i < n; i++)
	{
		for(auto &e : G[i]) indegree[e]++;  // i->eへの辺
	}
	queue<int> que;
	for(i = 0; i < n; i++)
	{
		if(indegree[i] == 0) que.push(i);
	}

	depth.resize(n, 0);
	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		result.push_back(v);
		for(auto &e : G[v])  // 頂点vを消すイメージで、vから出た先の頂点の入次数を1減らす
		{
			indegree[e]--;
			if(indegree[e] == 0)
			{
				que.push(e);
				depth[e] = depth[v]+1;
			}
		}
	}
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		g[x].push_back(y);
	}

	vector<int> result, depth;
	bfs_topological_sort(g, result, depth);
	cout << *max_element(depth.begin(), depth.end()) << endl;

	return 0;
}
