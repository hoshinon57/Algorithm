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
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC291 https://atcoder.jp/contests/abc291

/*
 * トポロジカルソート的な手法で解く。
 * 
 * BFSでトポロジカルソートを行うにあたり、入次数が0の頂点が常に1個のみ、というのが条件となる。
 * これを満たさない場合はNo回答。
 * 
 * 最後までトポロジカルソートできた場合はYes回答。
 * 結果が入る配列について、idxとvalが逆転しているイメージなので、これを反転して出力する。ここがややこしかった。
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
		// 入次数0の頂点は常に1個のみ
		if(que.size() != 1) break;

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
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		g[x].push_back(y);
	}
	vector<int> result;
	bfs_topological_sort(g, result);

	// トポロジカルソート的なことをしてみて、全頂点が入っていなかったらNG.
	if((int)result.size() != N)
	{
		cout << "No" << endl;
		return 0;
	}

	vector<int> ans(N);
	for(i = 0; i < N; i++)
	{
		// result[]の要素番号と値が逆転しているイメージ
		ans[result[i]] = i+1;  // +1は1-indexedへの変換
	}
	cout << "Yes" << endl;
	for(auto &e : ans) cout << e << " ";
	cout << endl;

	return 0;
}
