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
 * ABC013-D 連結成分が全てサイクル
 * ABC146-D
 * ABC199-D
 * ABC218-F 最短経路を復元
 * ABC202-E
 * ABC240-E
 * ABC251-F
 * ABC277-C 連想配列 or 座標圧縮
 * ABC282-D 二部グラフ
 * ABC285-D 連想配列 or 座標圧縮
 * ABC311-C
 * ABC315-E これ解けなかったのは大反省
 * ABC327-D 二部グラフ
 * ABC351-D グラフではない / DFSに連結成分の番号を持たせるテクニック
 * ABC373-D
 * ABC397-E 木DP 考察ミスってWAだった、大反省
 * 
 * 連想配列を使う場合のグラフの定義方法は、このフォルダにあるgraph.cppを参考のこと。
 */

using Graph = vector<vector<int>>;

// 実装テンプレートやライブラリ一覧
void dfs(Graph &graph, int v);  // [実装テンプレート]頂点vから深さ優先探索する
void dfs_tree_dp(Graph &g, int v, int p);  // [実装テンプレート]木上DPでの実装方針（の一例）
void dfs_tree(Graph &g, int v, int p);  // [実装テンプレート]木であるグラフにてDFS
void dfs_tree_depth(Graph &grp, vector<int> &d, int v, int p, int now);  // [ライブラリ]各頂点の深さを計算
int dfs_tree_subnum(Graph &grp, vector<int> &n, int v, int p);  // [ライブラリ]各頂点vを部分木としたときの頂点数計算
// [ライブラリ]頂点s,tのパス上の頂点一覧を求める
bool dfs_tree_path(Graph &grp, int s, int t, vector<int> &path, int rev, int p);
// [ライブラリ]頂点vを基点にオイラーツアー
void dfs_tree_euler_tour(Graph &g, int v, vector<int> &path, vector<int> &vin, vector<int> &vout);
// [ライブラリ]有向グラフにてサイクル検索
bool dfs_cycle_detection_directed(Graph &g, int v, vector<bool> &seen, vector<bool> &finished, vector<int> &history);
// [ライブラリ]無向グラフにてサイクル検索
bool dfs_cycle_detection_undirected(Graph &g, int v, int p /* =-1 */, vector<bool> &seen, vector<bool> &finished, vector<int> &history);
// [ライブラリ]二部グラフか判定
bool dfs_bipartite(Graph &g, int v, vector<int> &col, int cur);

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

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// p:vの親
// 呼び出し元からは dfs(g, 0, -1) や dfs(g, 0) などで呼び出す
void dfs_tree(Graph &g, int v, int p = -1)
{
	// ★問題に応じて、ここで何らか処理
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		// ★問題に応じて、ここで何らか処理
		dfs_tree(g, e, v);
	}
}

// vector<int> dp(N);
// [実装テンプレート]
//   木上のDPにてこの情報を使いたい場合に、まず子へ再帰して子のDP値を確定させ、
//   親(呼び出し元)にて子のDP値を参照するという実装方法。
//   これなら再帰関数で戻り値の設定は不要、というのがポイント。dp[]自体もグローバル変数にて実装しやすい(かも)。
void dfs_tree_dp(Graph &g, int v, int p = -1)
{
#if 0
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs_tree_dp(g, e, v);  // まずは子へ再帰してしまう
		// 子のDP値が確定しているので、親側にて自由に使う
		dp[v] += dp[e];
	}
#endif
}

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// 各頂点の深さ(vを0とする)をd[]に設定する
// 呼び出し元からは dfs(g, depth, 0) のように呼び出す
// p:vの親
// now:頂点vの時点の根からの深さ
void dfs_tree_depth(Graph &grp, vector<int> &d, int v, int p = -1, int now = 0)
{
	d[v] = now;
	for(auto &e : grp[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		dfs_tree_depth(grp, d, e, v, now+1);
	}
}

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// 各頂点vを部分木としたときの頂点数(vを含む)をn[]に設定する
// 呼び出し元からは dfs(g, sub_num, 0) のように呼び出す
// nは0で初期化しておくこと
// p:vの親
int dfs_tree_subnum(Graph &grp, vector<int> &n, int v, int p = -1)
{
	n[v]++;  // 自身のぶん
	for(auto &e : grp[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		n[v] += dfs_tree_subnum(grp, n, e, v);
	}
	return n[v];
}

// [verify]ABC267-F, ABC152-F
// 木であるグラフについて、頂点s,tのパスを探索し通る頂点一覧をpath[]に設定する
// 呼び出し元からは dfs(g, s, t, path) のように呼び出す
// 初回呼び出し時にs,tをswapするため(※)、もし有向グラフにて使いたい場合はrev=0として呼び出すこと
// (※)探索アルゴリズムより、pathにはt->s方向のパスが格納されるため
// s-tパスが存在しない場合は、pathに変化なし
// p:sの親
bool dfs_tree_path(Graph &grp, int s, int t, vector<int> &path, int rev = 1, int p = -1)
{
	if(rev == 1) swap(s, t);
	if(s == t)
	{
		path.push_back(s);
		return true;
	}
	for(auto &e : grp[s])
	{
		if(e == p) continue;
		if(dfs_tree_path(grp, e, t, path, 0, s))
		{
			path.push_back(s);
			return true;
		}
	}
	return false;
}

// 木gについて、頂点vを基点にオイラーツアーを実施。
// path, vin, voutは関数内でassign()するので未初期化でよい。(サイズは木の頂点数をNとして、pathは2N-1, vinとvoutはNになる)
// path:通った頂点の順番
// vin, vout:各頂点iについて、最初ないし最後に通ったタイミング
// 参考：
//   https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7
//   https://scrapbox.io/ganariya-competitive/%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E3%83%84%E3%82%A2%E3%83%BC%E3%80%80%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA
//   https://obsidian.bibliophage.jp/Article/%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E3%83%84%E3%82%A2%E3%83%BC/
//   https://drken1215.hatenablog.com/entry/2018/08/14/193500
void dfs_tree_euler_tour(Graph &g, int s, vector<int> &path, vector<int> &vin, vector<int> &vout)
{
	// [memo]
	// pathのサイズについて：
	//   辺ごとに行き帰りで2回処理される。
	//   辺の数は(N-1)なので2(N-1)回処理。それに基点vが1回足されるので、2(N-1)+1=2N-1.
	// 
	// 頂点vについてpath[]における区間[vin[v],vout[v]]が、頂点vを根とする部分木を表す区間となる。
	// つまり、根付き木に対するクエリはオイラーツアーにより「区間に対するクエリ」に変換される。
	// 
	// 隣り合った2頂点u,vがあるとき、vin[]が小さい方が根寄り。

	int N = g.size();
	path.assign(2*N-1, 0);
	vin.assign(N, 0);
	vout.assign(N, 0);

	auto dfs = [&](auto &self, int &ord, int v, int p = -1) -> void
	{
		path[ord] = v;
		vin[v] = vout[v] = ord;  // ここでvoutを設定するのは、葉である頂点用
		ord++;
		for(auto &e : g[v])
		{
			if(e == p) continue;
			self(self, ord, e, v);
			path[ord] = v;
			vout[v] = ord;
			ord++;
		}
	};
	int o = 0;
	dfs(dfs, o, s);
}

// 有向グラフにて、頂点vを起点にDFS.
// サイクルを見つけたらtrueを返す。その場合、vからサイクル終端までをhistoryに保持する。
//   historyの例：0->1->2->3->4->2  サイクルは2-3-4の部分。末尾の2が2回出てくることに注意すること。
// seen, finishedの要素数は頂点数で、falseで初期化しておくこと。
// 
// このDFSにて、
// (1)1つの連結成分に2つ以上のサイクルがある場合、
//    サイクル有無の判定は可能。
//    サイクル検出は何らか1つなら可能。全てのサイクル検出は不可能。
// (2)Functional Graphなら連結成分ごとにサイクルは1個なので、
//    呼び出し元で未探索の頂点からDFSすることで全てのサイクルを検出可能。
// 
// サイクルに入る前の頂点数をpre, サイクルのループ数をcycとすると、以下で求められる。
// pre = find(hist.begin(), hist.end(), hist.back()) - hist.begin();
// cyc = hist.size() - pre - 1;  // -1は上記例にて末尾の2が2回出てくるため
// サイクルのc番目(0-indexed)の頂点は hist[pre+c] となる
bool dfs_cycle_detection_directed(Graph &g, int v, vector<bool> &seen, vector<bool> &finished, vector<int> &history)
{
	seen[v] = true;
	history.push_back(v);  // vを行きがけ時に登録し、帰りがけ時に削除

	for(auto &e : g[v])
	{
		if(finished[e]) continue;
		if(seen[e] && !finished[e])
		{
			// サイクル検出した
			finished[v] = true;  // 頂点vについて探索完了
			history.push_back(e);  // サイクルの終端
			return true;
		}
		if(dfs_cycle_detection_directed(g, e, seen, finished, history))  // サイクル検出してreturnしてきた場合は、再帰的にreturnしていく
		{
			finished[v] = true;  // この場合も、頂点vについて探索完了扱いとする
			return true;
		}
	}

	finished[v] = true;
	history.pop_back();
	return false;
}

// 無向グラフにて、頂点vを起点にDFS.
// サイクルを見つけたらtrueを返す。その場合、vからサイクル終端までをhistoryに保持する。
//   historyの例：0->1->2->3->4->2  サイクルは2-3-4の部分。末尾の2が2回出てくることに注意すること。
// p:vの親で、逆流防止に使う。呼び出し元からは-1で指定すること。
// seen, finishedの要素数は頂点数で、falseで初期化しておくこと。
// 
// ★動作確認が甘いので使用時は注意。
// 
// このDFSにて、
// (1)1つの連結成分に2つ以上のサイクルがある場合、
//    サイクル有無の判定は可能。
//    サイクル検出は何らか1つなら可能。全てのサイクル検出は不可能。
bool dfs_cycle_detection_undirected(Graph &g, int v, int p /* =-1 */, vector<bool> &seen, vector<bool> &finished, vector<int> &history)
{
	seen[v] = true;
	history.push_back(v);  // vを行きがけ時に登録し、帰りがけ時に削除

	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		if(finished[e]) continue;
		if(seen[e] && !finished[e])
		{
			// サイクル検出した
			finished[v] = true;  // 頂点vについて探索完了
			history.push_back(e);  // サイクルの終端
			return true;
		}
		if(dfs_cycle_detection_undirected(g, e, v, seen, finished, history))  // サイクル検出してreturnしてきた場合は、再帰的にreturnしていく
		{
			finished[v] = true;  // この場合も、頂点vについて探索完了扱いとする
			return true;
		}
	}

	finished[v] = true;
	history.pop_back();
	return false;
}

// 頂点vを起点にDFSし、二部グラフにできればtrueを返す
// col[i]:頂点iの色 未探索の場合は-1 呼び出し元で-1で初期化しておくこと
// cur:
//   vに塗る色
//   基本0で呼び出して良いが、他の"偶数"cを指定すると c,c+1 で塗る
//   例：cur=2なら2,3, cur=4なら4,5
// 呼び出し元からは、未探索の頂点vについて(※)
//   (g, v, col)
// のように呼び出していくこと。 (※)col[v]が-1かどうかで判定できる
bool dfs_bipartite(Graph &g, int v, vector<int> &col, int cur = 0)
{
	col[v] = cur;
	for(auto &e : g[v])
	{
		if(col[e] != -1)  // もう塗られている
		{
			if(col[e] == col[v]) return false;
		}
		else  // まだ塗っていない
		{
			int nxt = cur^1;
			if(!dfs_bipartite(g, e, col, nxt)) return false;
		}
	}
	return true;
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

	// 以下は基本パターンでの実装。
	// 2つの頂点s->tへのパスが存在するかを判定する。
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