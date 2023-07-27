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

// ABC223 https://atcoder.jp/contests/abc223

/*
 * トポロジカルソートで解く。
 * 
 * 「AiはBiより先に現れる」を、Ai->Biへの有向辺を張ってトポロジカルソートする。
 * ここで問題となるのは以下2点。
 * (1)数列Pが存在しないケースの検出：
 *    これはトポロジカルソートした結果が格納された頂点が、グラフの頂点より少ないことで判断できる。
 * (2)辞書順で最小のものを求める：
 *    トポロジカルソートにて、キューから取り出す際に頂点番号が最小のものを取り出せばよい。
 *    これはpriority_queueを使うことで実現できる。
 */

using Graph = vector<vector<int>>;

// 自作のトポロジカルソートのライブラリを、queueからpriority_queueに変更した版
void bfs_topological_sort(Graph &G, vector<int> &result)
{
	int i;
	int n = G.size();

	vector<int> indegree(n);  // 入次数
	for(i = 0; i < n; i++)
	{
		for(auto &e : G[i]) indegree[e]++;  // i->eへの辺
	}
//	queue<int> que;
	priority_queue<int, vector<int>, greater<int>> que;
	for(i = 0; i < n; i++)
	{
		if(indegree[i] == 0) que.push(i);
	}

	while(!que.empty())
	{
//		int v = que.front();
		int v = que.top();
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
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
	}
	vector<int> result;
	bfs_topological_sort(g,result);

	// トポロジカルソートできない場合、size()で判断可能
	if(result.size() != g.size())
	{
		cout << -1 << endl;
		return 0;
	}

	for(i = 0; i < (int)result.size(); i++)
	{
		cout << result[i]+1;
		if(i != (int)result.size()-1) cout << " ";
	}
	cout << endl;

	return 0;
}
