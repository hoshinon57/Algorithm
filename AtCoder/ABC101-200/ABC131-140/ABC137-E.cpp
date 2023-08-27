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
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC137 https://atcoder.jp/contests/abc137

/*
 * (主に)ベルマン・フォード法で解く。
 * 
 * 「ゲーム終了時にT分*P枚のコインを支払う」とあるが、これは1辺ごとにP枚のコインを支払うと置き換えて良い。
 * 各辺の重みを C-P とすると、頂点1からNへの辺の重みの最大値を求めることになり、
 * さらに重みを -1倍すると、最短経路問題になる。
 * 
 * するとコインを無限に拾える閉路は負閉路になるが、ベルマン・フォード法を用いることで負閉路も検出できる。
 * (制約的にも間に合う)
 * 
 * ただし入力例3のように、負閉路が頂点1からNへの経路に影響しないケースもあり、これを判定する必要がある。
 * これは、逆方向に辺を張ったグラフにて頂点NからBFSし、
 * 負閉路の頂点にたどり着ければ「無限にコインを拾える」と判断できる。
 * 
 * ★間違った判定として、「もう一度ベルマン・フォード法を回す」がある。
 *   ただしこれは「負閉路があるものの、何周も回さないと影響しない」というケースにて正しく判定されない。
 *   https://twitter.com/c_Ry0ta/status/1160196135467618304
 *   https://blog.hamayanhamayan.com/entry/2019/08/14/131217
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};

// 重み付きグラフ
// 各頂点が持つのは、頂点ではなく辺
using Graph = vector<vector<Edge>>;

using Graph_BFS = vector<vector<int>>;
// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph_BFS &G, int s, vector<int> &dist)
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

int main(void)
{
	int i;
	ll N, M, P; cin >> N >> M >> P;
	Graph g(N);
	Graph_BFS g_bfs(N);
	for(i = 0; i < M; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		a--; b--;
		c = -(c-P);
		g[a].push_back(Edge(b, c));
		g_bfs[b].push_back(a);  // 逆方向
	}

	bool exist_negative_cycle = false;  // 負閉路があればtrue
	vector<int> ncycle_list;  // 負閉路に含まれる頂点
	vector<ll> dist(N, INF64);
	dist[0] = 0;
	for(i = 0; i < N; i++)  // 最大で(頂点数-1)回の反復が必要。(頂点数)回目のループは、負閉路の存在確認用。
	{
		bool update = false;  // 更新が発生したらtrue
		for(int v = 0; v < N; v++)
		{
			if(dist[v] == INF64) continue;  // 始点からまだ到達していない頂点はスルー
			for(auto &e : g[v])  // 頂点vから出ている各辺について探索
			{
				// v->e.to への更新チェック
				if(dist[e.to] > dist[v] + e.weight)
				{
					dist[e.to] = dist[v] + e.weight;
					update = true;
					if(i == N-1) ncycle_list.push_back(e.to);
				}
			}
		}

		// 更新がなければ完了している
		if(!update) break;

		// N回目の反復で更新があるならば、負閉路を持つということ
		if(i == N-1 && update) exist_negative_cycle = true;
	}

	// 負閉路があれば、頂点Nから逆方向にBFSし、負閉路となる頂点に到達できたら-1出力
	if(exist_negative_cycle)
	{
		vector<int> dist_bfs(N, INF32);
		bfs_graph(g_bfs, N-1, dist_bfs);
		for(auto &e : ncycle_list)
		{
			if(dist_bfs[e] != INF32)
			{
				cout << -1 << endl;
				return 0;
			}
		}
	}

	ll ans = max(-dist[N-1], 0LL);  // 取得コインよりも支払うコインの方が多くなるケースは、答は0.
	cout << ans << endl;

	return 0;
}
