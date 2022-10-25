#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題57
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2008yo/tasks/joi2008yo_f

/*
 * ヒープを用いたダイクストラ法で解く。
 * 
 * 2行目以降の入力について、最初の数字が
 *   1であれば、グラフに辺を追加する。問題文より、両方向に辺を張ることに注意。
 *   0であれば、ここまで作ったグラフを元にダイクストラ法で最小運賃を求める。
 * 
 * Dijkstra()の中身はいわゆる「教科書通りの実装」であり、説明は省略する。
 */

// 1つの辺を表す構造体
struct Edge {
	int to;  // 辺の行き先
	ll w;  // 辺の重み
	Edge(ll to_, ll w_) : to(to_), w(w_) {};
};

using Graph = vector<vector<Edge>>;

// 島sを始点とし、tへの最小運賃を返す
// tへたどり着けない場合はINF64を返す
ll Dijkstra(Graph &graph, int N, int s, int t)
{
	vector<ll> cost(N, INF64);  // cost[i]:島iへの最小運賃
	cost[s] = 0;

	using pil = pair<ll, int>;  // {ある島への最小運賃, その島の番号}
	priority_queue<pil, vector<pil>, greater<pil>> que;
	que.push({cost[s], s});

	while(!que.empty())
	{
		ll c = que.top().first;
		int v = que.top().second;
		que.pop();

		if(c > cost[v]) continue;

		for(auto &e : graph[v])  // e:頂点vから出る辺
		{
			if(cost[e.to] > cost[v] + e.w)
			{
				cost[e.to] = cost[v] + e.w;
				que.push({cost[e.to], e.to});
			}
		}
	}

	return cost[t];
}

int main(void)
{
	// 0-indexed
	int i;
	int N, K;
	cin >> N >> K;
	Graph graph(N);
	for(i = 0; i < K; i++)
	{
		int kind;
		cin >> kind;
		if(kind == 0)  // ここまでのgraphを使ってダイクストラ
		{
			int a, b;
			cin >> a >> b;
			a--;  // 0-indexedに変換
			b--;
			ll answer = Dijkstra(graph, N, a, b);
			if(answer != INF64) cout << answer << endl;
			else cout << -1 << endl;
		}
		else  // graphに辺を追加
		{
			int c, d, e;
			cin >> c >> d >> e;
			c--;  // 0-indexedに変換
			d--;
			// 両方向に辺を張る
			graph[c].push_back({d, e});  // c->d 運賃e
			graph[d].push_back({c, e});  // d->c 運賃e
		}
	}

	return 0;
}
