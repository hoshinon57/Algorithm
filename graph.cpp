#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// グラフ問題の解き方のテンプレート

/*
 * [関連する問題]
 * ABC259-D
 * ABC257-D
 * ABC244-E
 * ABC237-E
 * ABC231-D サイクル検出
 */

/*
 * [用語や定義]
 * 木：グラフのうち、連結でサイクルを持たないもの
 * 多重辺：複数本の辺が同一の頂点間を結んでいるもの
 * 自己ループ：e=(v,v)のように辺が同じ頂点を結んでいるもの
 * 単純グラフ：多重辺も自己ループも持たないグラフ
 * 連結：無向グラフGの任意の2頂点s,t∈Vに対してs-tパスが存在するとき、Gは連結
 * 二分木：すべての頂点に対して高々2個の子頂点しかもたない順序木
 * 強平衡二分木：二分木であり、すべての葉の深さが高々1しか違わないもの
 */

using Graph = vector<vector<int>>;
// Graph graph(N);
// で定義する

void cycle(int N, Graph &graph);

// 基本パターン
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
	// サイクルを検出したい場合の処理
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

// サイクルを検出したいパターン
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
