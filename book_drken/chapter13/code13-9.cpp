#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// code13.9の写経

using Graph = vector<vector<int>>;

vector<int> depth;  // depth[v]:頂点vの深さ
vector<int> subtree_size;  // subtree_size[v]:頂点vを根としたときの部分木のサイズ v自身を含む

// 再帰を用いた深さ優先探索
// 各頂点の深さおよび部分木サイズを求める
// v:探索する頂点
// parent:vの親 vが根のときは-1
// d:vの深さ
void dfs(Graph &graph, int v, int parent, int d)
{
	// 以下、subtree_size[]は本の説明である帰りがけ時ではなく、再帰呼び出し直後に更新した。
	// 帰りがけ時の方がロジックとしては分かりやすい気はする。
	// 一方で、以下コードの方が実装はシンプルな気もする。

	depth[v] = d;
	subtree_size[v] = 1;  // v自身のぶん

	// 頂点vから行ける各頂点childについて、再帰的に探索
	for(auto &child : graph[v])
	{
		if(child == parent) continue;  // 親方向への逆流を防ぐ
		dfs(graph, child, v, d+1);  // 今度はvを親にして再帰
		subtree_size[v] += subtree_size[child];  // 部分木サイズを加算
	}
}

int main(void)
{
	// 以下、0-indexedかつ無向グラフ
	int i;
	int N;  // N:頂点数 木のため辺の数はN-1としてよい
	cin >> N;

	Graph graph(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}

	// 探索
	int root = 0;  // 仮に頂点0を根とする
	depth.assign(N, 0);
	subtree_size.assign(N, 0);
	dfs(graph, root, -1, 0);

	for(i = 0; i < N; i++)
	{
		cout << i << ": depth=" << depth[i] << ", subtree_size=" << subtree_size[i] << endl;
	}

	return 0;
}
