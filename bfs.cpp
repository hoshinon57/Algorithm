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
 * ABC383-C 多始点BFS
 * ABC363-E
 * ABC361-D 文字列でのBFS 良問
 * ABC348-D 実装少し重い 良問
 * ABC339-D グリッドグラフ上にプレイヤー2人
 * ABC320-D
 * ABC317-E
 * ABC315-E これ解けなかったのは大反省
 * ABC311-D
 * ABC309-D
 * ABC308-D
 * ABC302-F 超頂点
 * ABC301-E
 * ABC299-E
 * ABC292-E
 * ABC292-D
 * ABC289-E 移動するものが2つ
 * ABC285-D サイクル検出, 連想配列
 * ABC277-C
 * ABC272-D
 * ABC269-D
 * ABC259-D
 * ABC257-F 超頂点
 * ABC257-D
 * ABC251-F
 * ABC244-F bitDPっぽいのをBFSで解く
 * ABC241-F 
 * ABC232-D
 * ABC231-D サイクル検出
 * ABC226-E
 * ABC224-D 頂点を文字列で、距離をmapで管理
 * ABC223-D トポロジカルソート
 * ABC216-D トポロジカルソート(別解)
 * ABC197-F 移動するものが2つ
 * ABC184-E 通常のBFSと0-1 BFS どちらでも解ける
 * ABC168-D
 * ABC142-F サイクル検出＆復元
 * ABC139-E トポロジカルソート＆最長経路
 * ABC132-E
 * ABC277-E 0-1 BFS
 * ABC246-E 0-1 BFS
 * ABC213-E 0-1 BFS
 * ABC176-D 0-1 BFS
 * ARC005-C 0-1 BFS
 * AGC033-A 多始点BFS
 * 典型90-43 0-1 BFS
 * 典型90-54 超頂点 https://atcoder.jp/contests/typical90/tasks/typical90_bb
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
// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist)
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

// vector<string> mp;
// 
// ABC176-D Wizard in Mazeの入力を元に実装している。(https://atcoder.jp/contests/abc176/tasks/abc176_d)
// 特にコスト1の部分について、問題に合わせて書き換えること。
// -----
// 高さH, 幅Wのグリッドグラフについて、(y,x)=(sy,sx)を起点に0-1 BFS.
// distは要素数[H][W]で、未探索部分はINFで埋めていること。
// 探索結果として、distに(sy,sx)を起点(=0)とした距離を格納する。
void bfs01_grid_graph(int H, int W, int sy, int sx, vector<vector<int>> &dist)
{
	deque<pair<int,int>> que;
	vector<vector<bool>> seen(H, vector<bool>(W, false));  // 処理済みならtrue

	dist[sy][sx] = 0;
	que.push_front({sy, sx});

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop_front();

		// 同じ頂点が「コスト0のケース」と「コスト1のケース」で2回キューに積まれることがあるため、ここで処理を1回に抑える
		// ただしこの処理は無くてもACする
		// あった方が無駄な探索が減るので実行時間は早くなる
		if(seen[y][x]) continue;
		seen[y][x] = true;

		// 上下左右マス コスト0
		for(int i = 0; i < DIR; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			// 壁など 問題によって書き換える
			if(mp[ny][nx] == '#') continue;

			// 距離を更新できるなら、更新してキューに積む
			if(dist[ny][nx] > dist[y][x])
			{
				dist[ny][nx] = dist[y][x];
				que.push_front({ny, nx});  // コスト0は先頭に追加
			}
		}
		// ワープ コスト1
		for(int ny = y-2; ny <= y+2; ny++)
		{
			for(int nx = x-2; nx <= x+2; nx++)
			{
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				// 壁など 問題によって書き換える
				if(mp[ny][nx] == '#') continue;

				// 距離を更新できるなら、更新してキューに積む
				if(dist[ny][nx] > dist[y][x] + 1)
				{
					dist[ny][nx] = dist[y][x] + 1;
					que.push_back({ny, nx});  // コスト1は末尾に追加
				}
			}
		}
	}
	return;
}

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
