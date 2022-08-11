#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 章末問題13.5

/*
 * 深さ優先探索にて解く。
 * 
 * dfsの開始時にtrueにするseen[]と、終了時にtrueにするfinished[]を用意する。
 * dfsの途中で
 *   seen[]はtrue, finished[]はfalse
 * となる頂点があれば、サイクルが発生しているとみなせる。
 * 
 * 「サイクルを具体的に1つ求めよ」という問題なので、
 *   サイクルのスタート地点 cycle_start
 *   有効サイクルの頂点候補を列挙する cycle[]
 * を用意し、サイクル検出時に設定していく。
 * 出力時は、cycle[]のうちcycle_startの出現以降の頂点を出力していく必要がある。
 * 
 * ちなみに解説記事から飛ぶQiitaの記事では、無向グラフだったり別問題のソースコードだったりする点に注意。
 */

using Graph = vector<vector<int>>;

vector<bool> seen;  // 探索の開始時にtrue
vector<bool> finished;  // 探索の終了時にtrue
vector<int> cycle;  // 有効サイクルの頂点(の候補)を列挙  サイクル外の頂点も積まれる可能性がある
int cycle_start;  // サイクルのスタート地点

bool dfs(Graph &graph, int v)
{
	seen[v] = true;
	for(auto &e : graph[v])
	{
		if(seen[e] && finished[e]) continue;  // 探索完了した頂点はスルー
		// 行きがけ順は完了かつ帰りがけ順は未完了、ならばサイクル検出とみなせる
		if(seen[e] && !finished[e])
		{
			cycle.push_back(e);
			cycle.push_back(v);
			cycle_start = e;
			return true;
		}
		
		if(dfs(graph, e))
		{
			// サイクル検出して戻ってきたら、今の頂点を積んでまた返す
			cycle.push_back(v);
			return true;
		}
	}
	finished[v] = true;

	return false;
}

int main(void)
{
	// 以下、0-indexedかつ有向グラフ
	int i;
	int N, M;  // N:頂点数 M:辺の数
	cin >> N >> M;

	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
	}

	// 探索
	seen.assign(N, false);
	finished.assign(N, false);
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 探索済みならスルー
		if(dfs(graph, i)) break;  // サイクルを見つけたら終了
	}

	reverse(cycle.begin(), cycle.end());  // 逆順にする
	bool disp = false;
	for(auto &e : cycle)
	{
		// cycle[]はサイクル以外の頂点を含む可能性がある。
		// cycle_startの頂点が出るまでは表示をスルーする必要がある。
		if(e == cycle_start) disp = true;
		if(disp)
		{
			cout << e << "->";
		}
	}
	cout << endl;

	return 0;
}
