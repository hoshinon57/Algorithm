#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 深さ優先探索のメモやテンプレート

/*
 * 再帰を用いて解いていく。
 * 本ソースコードでは基本パターンでの実装として、2つの頂点s->tへのパスが存在するかを判定する。
 * 
 * 以下は、ABC277-EでDFSで解こうとしてTLEとなった大反省を元に記載した。
 * ----------------------------------------------------------------
 * DFSとBFSの最大の違いは、探索の順序。
 * そのため探索の順序によらないものは、どちらを採用しても良い。
 * 参考：https://atcoder.jp/contests/abc277/editorial/5209
 * 
 * ★グラフ探索の考え方として、同じ頂点を2回以上探索するとTLEになる。（ようなテストケースが用意されている）
 * DFS：
 * (1)木上のグラフは、同じ頂点を2回以上通らないのでDFSでもOK.
 * (2)グリッドグラフや一般的なグラフについては、
 *    「あるマスに行けるか？」「グラフの連結数は？」といった問題であれば
 *    同じ頂点を2回以上通る必要がなく、used変数で探索を打ち切りできるため、DFSでもOK.
 * (3)ある頂点への最短距離を求めるのは、同じ頂点を2回以上通りうるのでDFSでは不適切。（WAではないが、TLEになる）
 *    o--o--o--o--o
 *    |           |
 *    S-----------o--o--T
 *    上記でS->Tへの最短距離を求める場合、上ルートの探索後に下ルートで再探索する可能性があり、ムダとなる。
 *    このようなケースはBFSを用いる。（もしくはダイクストラ法や0-1BFS）
 * 
 * BFS：
 * (4)最短距離を求める問題にて、辺の重みが2種類以上ある場合は、単なるBFSでは不適切。（WAではないが、TLEになる）
 *    理由は(3)のDFSと同じく再探索が生じうるから。
 *    ダイクストラ法や0-1BFSを使う。
 * ----------------------------------------------------------------
 * 
 * [関連する問題] グラフを深さ優先探索で解くような問題(BFSやUnionFindで解くことも可能)
 * AOJ 1160 https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1160&lang=ja
 * ABC277-C 連想配列 or 座標圧縮
 */


// 以下は基本パターンでの実装。
// 2つの頂点s->tへのパスが存在するかを判定する。

using Graph = vector<vector<int>>;
vector<bool> seen;  // seen[i]:頂点iが探索済みならtrue

// 頂点vから深さ優先探索する
void dfs(Graph &graph, int v)
{
	// ★問題によっては、範囲外または障害物ならreturnするなどの処理を入れる
	/*
	if(y < 0 || y >= H || x < 0 || x >= W) return;
	if(wall[y][x]) return;
	*/

	if(seen[v]) return;  // 探索済みならスルー
	seen[v] = true;

	// ★問題に応じて、ここで何らか処理

	for(auto &e : graph[v])  // e：頂点vから行ける頂点
	// ★グリッドグラフの場合、for(i = 0; i < 4; i++) として上下左右4方向を探索するケースもある
	{
		dfs(graph, e);
	}
}

int main(void)
{
	int i;
	int N, M;  // グラフの頂点数と辺の数
	cin >> N >> M;
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		// 0-indexedなら a--; b--; の処理を追加する
		graph[a].push_back(b);
		graph[b].push_back(a);  // 無向グラフの場合
	}

	seen.assign(N, false);  // 全頂点を未探索に初期化
	int s = 0;  // 始点
	int t = N-1;  // 終点

	dfs(graph, s);

	if(seen[t])
	{
		// 終点にたどり着ける
		cout << "Yes" << endl;
	}
	else
	{
		// 終点にたどり着けない
		cout << "No" << endl;
	}

	return 0;
}