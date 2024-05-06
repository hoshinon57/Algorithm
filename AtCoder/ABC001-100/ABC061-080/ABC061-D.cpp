#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC061 https://atcoder.jp/contests/abc061

/*
 * WAを修正しきれず、解法を見た。
 * ベルマン・フォード法を用いて解く。
 * 
 * 入力された重みcを-1倍することで、各頂点への最短路を求める問題に置き換えることができ、
 * ベルマン・フォード法を用いることができる。
 * これによりinf出力も、負閉路を検出したかどうかで判定することができる。
 * 
 * ただし負閉路の扱いに注意が必要で、
 *   頂点1から頂点Nの経路上にない負閉路は、無視しないといけない
 * ということ。
 * (ベルマン・フォード法を単純に実装すると、グラフ上のどこかに負閉路があるかどうかだけを判定するため)
 * 
 * よって本問題では、負閉路の判定条件を以下のようにする。(ここが自力で解けなかった)
 * ・反復回数をN回から2*N回に増やす。
 *   負閉路が頂点Nに影響を及ぼすのはN回では足りないケースがあり、
 *   一方で2*N回行えば確実に影響を及ぼせるため。
 *   参考：
 *     https://scrapbox.io/hkurokawa-cp/ABC_061_D_-_Score_Attack
 *     https://drken1215.hatenablog.com/entry/2019/02/16/075900
 * ・頂点Nについて、N回目【以降】の反復にて更新されたら、負閉路ありと判定する。
 */

struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};

// 重み付きグラフ
// 各頂点が持つのは、頂点ではなく辺
using Graph = vector<vector<Edge>>;

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		ll w;
		cin >> a >> b >> w;
		a--;  // 0-indexedに変換
		b--;
		w = -w;  // スコアを-1倍することで、最小値を求める問題にする
		graph[a].push_back(Edge(b, w));  // 辺を持つ
	}

	// ベルマン・フォード法を用いて解く
	bool exist_negative_cycle = false;  // 負閉路があればtrue
	vector<ll> dist(N, INF64);  // 始点から各頂点への距離
	dist[0] = 0;  // 始点は頂点1で固定 距離0にする
	for(i = 0; i < N*2; i++)  // 反復をN回ではなく2*N回にする
	{
		bool update = false;
		for(int v = 0; v < N; v++)
		{
			if(dist[v] == INF64) continue;  // 始点からまだ到達していない頂点はスルー
			for(auto &e : graph[v])
			{
				// v->e.to への更新チェック
				if(dist[e.to] > dist[v] + e.weight)
				{
					dist[e.to] = dist[v] + e.weight;
					update = true;

					// 始点から終点の間に負閉路があるかを求める
					// 終点がN回目以降の反復にて更新されたら、負閉路を持つと判断する
					if(e.to == N-1 && i >= N-1) exist_negative_cycle = true;
				}
			}
		}

		// 更新がなければ完了している
		if(!update) break;
	}

	if(exist_negative_cycle)
	{
		// スコアを-1倍しているので、負閉路がある場合がinf出力となる
		cout << "inf" << endl;
	}
	else
	{
		// 頂点Nまでの距離が、求めるスコアになる
		// -1倍して出力
		cout << dist[N-1]*(-1) << endl;
	}

	return 0;
}
