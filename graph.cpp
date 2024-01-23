#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// グラフ問題の解き方のテンプレート

/*
 * 「この問題ではDFS,BFS,またはダイクストラ法や0-1BFSのどれを使うべきか？」
 * を整理したものをdfs.cppに記載した。
 * ★グラフ探索の考え方として、同じ頂点を2回以上探索するとTLEになる。（ようなテストケースが用意されている）
 * DFS:最短距離を求めるケースに使うのは不適切。
 * BFS:最短距離の問題にて、辺の重みが複数あるようなグラフにて使うのは不適切。
 * 
 * [関連する問題]
 * ABC299-E
 * ABC292-E
 * ABC292-D
 * ABC289-E 移動するものが2つ
 * ABC277-E ダイクストラ、0-1BFS
 * ABC259-D
 * ABC257-D
 * ABC252-E
 * ABC244-E
 * ABC237-E
 * ABC231-D サイクル検出
 */

/*
 * ベルマン・フォード法、ダイクストラ法、ワーシャルフロイド法などは
 * フォルダ /book_drken/chapter14 のソースコード(code14-4.cpp)、およびchapter14-memo.txtを参照。
 * ダイクストラ法のライブラリはdijkstra.cppを参照。
 * ワーシャルフロイド法のライブラリはwarshall-floyd.cppを参照。
 */

/*
 * [用語や定義]
 * 木：グラフのうち、連結でサイクルを持たないもの
 *     木ならば (辺の数)=(頂点の数)-1 となる
 * 多重辺：複数本の辺が同一の頂点間を結んでいるもの
 * 自己ループ：e=(v,v)のように辺が同じ頂点を結んでいるもの
 * 単純グラフ：多重辺も自己ループも持たないグラフ
 * 連結：無向グラフGの任意の2頂点s,t∈Vに対してs-tパスが存在するとき、Gは連結
 * 直径：グラフの2点間の距離の最大値
 * 二分木：すべての頂点に対して高々2個の子頂点しかもたない順序木
 * 強平衡二分木：二分木であり、すべての葉の深さが高々1しか違わないもの
 * 全域木：連結重み付きの無向グラフG=(V,E)について、
 *         Gの部分グラフであって木であるもののうち、Gの全頂点をつなぐもの
 */

using Graph = vector<vector<int>>;
// Graph graph(N);
// で定義する

/*
 * グラフを Graph = vector<vector<int>> で表現できない場合：
 * 例：文字列や、数値であっても10^9などメモリが足りないケース
 * 
 * 連想配列を使って
 *   map<string, vector<string>> mp;
 *   map<int, vector<int>> mp;
 * のように管理すれば、int型のケースとほぼ同じ実装で可能。 (vector<map<... のようにvectorでくくる必要は無い)
 * 探索済みを表すseen[]も、
 *   map<int, bool> seen;
 * の形で。
 * ※各頂点に対して範囲forでアクセスしたい場合は、以下のようにe.firstでアクセスする点が異なる。
 *   for(auto &e : mp)
 *     if(seen[e.first]) continue;
 * 
 * [注意]
 * ただしmapだと計算量の定数倍部分が遅くなり、TLEになるケースもありそう。
 * 参考：ABC289-E
 * 
 * この場合、入力時（グラフ構築時）のタイミングで
 *   seen[a] = false;  // 未探索
 * として明示的にmapの要素を作成しておくと、探索時にfind()やcount()で要素の存在判定が不要になるので楽。
 * 
 * もしくは連想配列ではなく、座標圧縮して頂点番号を0から順に振っていく方法もありそう。
 * 
 * 参考：
 * ABC277-C https://atcoder.jp/contests/abc277/tasks/abc277_c
 * ABC285-D https://atcoder.jp/contests/abc285/tasks/abc285_d
 */

/*
 * サイクルの存在判定や検出など メモ
 * (1)無向グラフ - サイクルの存在判定
 *    seenを使ったDFS
 *    UnionFind (これが手っ取り早いかな)
 * 
 * (2)有向グラフ - サイクルの存在判定
 *    seen/finishedを使ったDFS
 * 
 * (3)無向/有向グラフ - サイクル検出(存在判定と、サイクルに含まれる頂点の列挙)
 *    seen/finished/historyを使ったDFS
 *    ⇒参考：
 *     dfs.cpp - dfs_cycle_detection_directed(), dfs_cycle_detection_undirected()
 *     https://drken1215.hatenablog.com/entry/2023/05/20/200517 
 *    ・1つの連結成分に2つ以上のサイクルがある場合、サイクルの存在判定は可能。
 *      サイクル検出は1つなら可能。
 *      全てのサイクル検出は不可能。
 *    ・Functional Graphのように「連結成分ごとにサイクルは1つ」(もしくは1個以下)であれば、全てのサイクルを検出可能。
 *      未探索の頂点からDFSしていけばよい。
 *      参考：ABC256-E.cpp
 *    ※「何らかのサイクルに含まれる頂点を全て列挙」なら強連結成分分解というやつ？
 * 
 * ※「サイクルが何個あるか？」は現実的な時間では解けないみたい。
 *   参考：https://twitter.com/kyopro_friends/status/1668203146957709312
 */

/*
 * トポロジカルソート：
 * 与えられた有向グラフに対し、各頂点を辺の向きに沿うよう順序付けること。(けんちょん本 13.9章より)
 * ・グラフGがトポロジカルソートできること
 * ・グラフGに有向サイクルが無いこと   ※Directed Acyclic Graph(DAG)とよぶ
 * 上記2つは同値関係となる。
 * 
 * BFSでトポロジカルソートを求めるライブラリはbfs.cppを参照。
 * 
 * 関連問題：
 *   AOJ GRL_4_B https://onlinejudge.u-aizu.ac.jp/problems/GRL_4_B
 *   ABC223-D
 *   ABC291-E
 * 参考：
 *   https://algo-logic.info/topological-sort/
 */

void cycle(int N, Graph &graph);

// 基本パターン(無向グラフ)
int main(void)
{
	// 以下、0-indexed
	int i;
	int N, M;  // N:頂点数 M:辺の数
	cin >> N >> M;
	Graph graph(N);
	// まずは各頂点ごとの接続を設定する
	// graph[0]には、頂点0(入力としては頂点1)と繋がっている頂点が列挙される
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		a--;  // 0-indexedに変換
		b--;
		graph[a].push_back(b);
		graph[b].push_back(a);  // 無向グラフの場合、反対方向も設定する
	}

#if 0
	// 無向グラフでのサイクルを検出したい場合の処理
	cycle(N, graph);
	return 0;
#endif

	// ★以下の処理では、頂点0(入力としては頂点1)と繋がっている頂点を全列挙する
	queue<int> que;
	vector<bool> seen(N, false);  // seen[i]:頂点iが探索済みならtrue
	que.push(0);  // 頂点0を起点として探索する  ★ここを問題に合わせて実装する
	seen[0]= true;  // 頂点0は探索済み
	while(!que.empty())
	{
		// キューから頂点を1つ取得し、それをキューから削除する
		int v = que.front();
		que.pop();
		// ★問題に応じて、ここで何らか処理
		
		for(auto &e : graph[v])  // 頂点vと接続している頂点を、1つずつeに設定
		{
			if(seen[e]) continue;  // 探索済みならスルー
			/*
			 * おそらく問題次第と思うが、seen[]はここで更新しておく。
			 * キューから取り出しのタイミング(ここより遅くなる)で更新するとなると、
			 * テストケースによってはeのキュー積みが大量に発生し、TLEになってしまう。(参考：ABC259-D.cpp)
			 * 
			 * DFSだとここの処理のすぐ後にeに関する処理を行うことになり、そこがBFSとのタイミングの違い。
			 */
			seen[e] = true;
			que.push(e);
		}
	}

	for(i = 0; i < N; i++)
	{
		if(seen[i]) cout << i+1 << " ";		
	}
	cout << endl;

	return 0;
}

// 無向グラフでのサイクルを検出したいパターン
// seen[]とは別に、キュー積み時に頂点の親を設定する
void cycle(int N, Graph &graph)
{
	int i;
	vector<bool> seen(N, false);
	// 各頂点ごと、そこを起点とした幅優先探索を行う
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 起点が探索済みならスルー
		queue<pair<int,int>> que;  // {探索する頂点, その親}  第2引数が-1ならば、親無し
		seen[i] = true;
		que.push(make_pair(i,-1));  // 頂点iを起点とするので、第2引数は-1(親無し)
		while(!que.empty())
		{
			// キューから頂点を1つ取得し、vに設定
			pair<int,int> v;
			v = que.front();
			que.pop();
			for(auto &e : graph[v.first])  // 頂点vと接続している頂点を、1つずつeに設定
			{
				if(e == v.second) continue;  // eがvの親ならスルー
				if(seen[e])
				{
					// ★ループ発生
					break;
				}
				seen[e] = true;
				que.push(make_pair(e, v.first));
			}
		}
	}
	// ★上記「ループ発生」を通ればサイクル有り、通らなければサイクル無し
}
