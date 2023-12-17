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

// ABC216 https://atcoder.jp/contests/abc216

/*
 * トポロジカルソートで解く。
 *   https://www.youtube.com/watch?v=Tgd_zLfRZOQ  のD別解
 *   https://kanpurin.hatenablog.com/entry/2021/08/29/230502
 * 各筒についてa[i]->a[i-1]へ辺を張ったグラフについて、DAGとなっていればOK.
 * トポロジカルソートができるかどうかで判定する。
 */

using Graph = vector<vector<int>>;
// サイクルの無い有向グラフGについて、トポロジカルソートを行う。
// ソートした結果の頂点をresultに格納する。
// ※トポロジカルソートできない場合(サイクルがある場合)、G.size()とresult.size()が不一致かどうかで判断可能。
// 参考：
//   https://algo-logic.info/topological-sort/
//   https://qiita.com/Morifolium/items/6c8f0a188af2f9620db2
void bfs_topological_sort(Graph &G, vector<int> &result)
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

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		result.push_back(v);
		for(auto &e : G[v])  // 頂点vを消すイメージで、vから出た先の頂点の入次数を1減らす
		{
			indegree[e]--;
			if(indegree[e] == 0) que.push(e);
		}
	}
}

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int k; cin >> k;
		int pre = -1;
		for(j = 0; j < k; j++)
		{
			int a; cin >> a;
			a--;
			if(j == 0) {;}  // 末尾から辺は張らない
			else
			{
				g[a].push_back(pre);
			}
			pre = a;
		}
	}

	vector<int> res;
	bfs_topological_sort(g, res);
	YesNo(g.size() == res.size());

	return 0;
}
