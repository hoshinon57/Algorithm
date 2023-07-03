#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 幅優先探索のメモ

/*
 * キューを使って解いていくことが多い。
 * テンプレートとしてgraph.cppを参照。
 * 
 * graph.cppのテンプレート以外では、迷路の問題を幅優先探索で解くことがある。(こちらもキューを使う)
 * ARC005-C.cppやアルゴリズム本の13章末問題13.4(problem13-4.cpp)を参照。
 * 
 * 主に、重み無しグラフ(移動コストが0)の場合に、キューを用いた単純なBFSで解ける問題となる。
 * 迷路を解く問題や、2頂点が連結しているかを判定する問題など。
 * 各辺のコストが異なる場合は、ベルマン・フォード法やダイクストラ法を使うことになる。
 * ※これらの問題も単純なBFSで解くことはできるが、無駄な探索が増えてTLEになりやすい。
 *   例：ABC237のE問題について、単純なBFSで解いたらTLEになり(ABC237-E.cpp)、
 *       ダイクストラ法で解いたらACとなった(ABC237-E-2.cpp)
 * 移動コストが0 or 1のケースは、0-1 BFSも使える。
 * 
 * [関連する問題]
 * ABC308-D
 * ABC299-E
 * ABC292-E
 * ABC292-D
 * ABC289-E 移動するものが2つ
 * ABC285-D サイクル検出, 連想配列
 * ABC277-C
 * ABC272-D
 * ABC269-D
 * ABC259-D
 * ABC257-D
 * ABC232-D
 * ABC231-D サイクル検出
 * ABC226-E
 * ABC132-E
 * ABC277-E 0-1 BFS
 * ARC005-C 0-1 BFS
 * 
 * 0-1 BFSについて：
 * 重み付きグラフの辺の重みが0 or 1、または移動コストが0 or 1のときに使える手法。
 * 両端キューを使い、重みが0であればキューの先頭に、1であれば末尾に追加する。
 * 以下URLを参考。
 *   https://betrue12.hateblo.jp/entry/2018/12/08/000020
 *   https://drken1215.hatenablog.com/entry/2021/07/30/024800
 * 
 */

using Graph = vector<vector<int>>;
// 頂点数Nのグラフについて、頂点sを起点にBFS.
// distは要素数Nで、未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int N, int s, vector<int> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}
	return;
}

vector<string> mp;

// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点にBFS.
// distは要素数[H][W]で、未探索部分はINF32で埋めていること。
// 探索結果として、distに(sy,sx)を起点(=0)とした距離を格納する。
void bfs_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist)
{
	queue<pair<int,int>> que;

	dist[sy][sx] = 0;
	que.push({sy, sx});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();
		for(int i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(dist[ny][nx] != INF32) continue;

			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			dist[ny][nx] = dist[y][x] + 1;
			que.push({ny, nx});
		}	
	}
	return;
}

int main(void)
{
	/*
	連結成分の数を求めたい場合、以下の実装イメージとなる。
	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			if(mp[y][x] = (通れるマス) && dist[y][x] == INF32)  // 通れるマスで、かつBFS未実施のマス
			{
				bfs_grid_graph(H, W, y, x, dist);
				ans++;
			}
		}
	}
	*/
	return 0;
}
