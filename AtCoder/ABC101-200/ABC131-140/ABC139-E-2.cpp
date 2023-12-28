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

// ABC139 https://atcoder.jp/contests/abc139

/*
 * 各種解説を参考に、グラフで解く。
 *   公式解説PDF
 *   公式解説YouTube
 *   https://scrapbox.io/Example0911/ABC139_E_League
 * 
 * 2選手(i,j)の試合にIDを振っていく。
 * そして各選手ごとにN-1個あるIDについて、ID1<ID2<ID3,...,ID(N-1) という不等式を作る。
 * これを全選手について作り、不等式を満たすならば試合可能。
 * 
 * この不等式はグラフに帰着して解くことができる。
 * ID1->ID2->ID3... という辺を張っていくと、全体としてDAGになっていればよい。
 * DAGであれば最長経路が答になる。(今回の問題と自分のライブラリの場合、+1が必要)
 * 
 * 試合にIDを振り、グラフを構築する部分の方が実装大変だった。いやー苦手。
 */

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
	int i, j;
	int N; cin >> N;
	vector<vector<int>> a(N, vector<int>(N-1, 0));  // a[N][N-1]
	for(i = 0; i < N; i++)  // 選手iの、j番目の試合相手
	{
		for(j = 0; j < N-1; j++) { cin >> a[i][j]; a[i][j]--; }
	}
	int cnt = 0;
	vector<vector<int>> id(N, vector<int>(N));  // id[N][N] // 選手(i,j)の試合の番号
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N-1; j++)
		{
			if(i >= a[i][j]) continue;  // 選手(i,X)との試合にて、i<Xのものだけ見る
			id[i][a[i][j]] = cnt++;
		}
	}

	Graph g(cnt);
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N-1-1; j++)  // 人iについて、j番目の試合から(j+1)番目の試合について辺を張る
		{
			int x = i;
			int y = a[i][j];
			if(x >= y) swap(x, y);  // xの方を小さくする
			int x2 = i;
			int y2 = a[i][j+1];
			if(x2 >= y2) swap(x2, y2);
			g[id[x][y]].push_back(id[x2][y2]);
		}
	}

	vector<int> result, depth;
	bfs_topological_sort(g, result, depth);
	int ans = -1;
	if(g.size() == result.size())  // DAG
	{
		ans = *max_element(depth.begin(), depth.end()) + 1;
	}
	cout << ans << endl;

	return 0;
}
