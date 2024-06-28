#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC280 https://atcoder.jp/contests/abc280

/*
 * ポテンシャル的な考え方で解く。以下解説の通り。
 *   https://x.com/kyopro_friends/status/1599040351867895808
 * 
 * まず、X,Yが非連結のときはnanとなる。これはUnionFindで解ける。
 * 以下は同じ連結成分として記載する。
 * (問題文より、2頂点の双方向に辺が張られているため、同じ連結成分であれば必ず行き来できる)
 * 
 * 連結成分ごと、適当な頂点を基準とする。(ポテンシャル=0)
 * そのからポイントC増加でたどれる頂点の基準をCとし、逆にC減少では-Cとする。
 * これを各頂点,辺ごとに見ていったときに、基準にズレが発生したとすると、
 * そこをグルグル回り続けることで、その連結成分についてはポイントをinfにすることが可能。
 * ズレが無ければ、ポイントは有限の範囲に収まる。
 * 
 * 解説ではポテンシャルの計算をDFS/BFSで行っていたが、
 * 自分は用意しているライブラリの関係からダイクストラで求めた。
 * ある辺を使って次の頂点を探索するときに、dist[e.to]の値に応じて以下のように判定できる。
 * ・INF：初めて到達するのでいつも通りキューに入れる
 * ・dist[v]+e.wと等しい：別ルートでも到達可能で、矛盾はない
 * ・dist[v]+e.wと不一致：別ルートでも到達可能で、矛盾発生
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;
// 頂点sを起点にダイクストラ。
// distは未探索部分をINFで埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
// ABC280-F:矛盾があればtrueを返す
bool dijkstra(Graph &G, int s, vector<ll> &dist)
{
	dist[s] = 0;
	using dist_v = pair<ll, int>;  // 距離とその頂点番号のpair
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;
	que.push({dist[s], s});

	while(!que.empty())
	{
		ll d = que.top().first;
		int v = que.top().second;
		que.pop();

		if(d > dist[v]) continue;  // {d, v}はゴミのためスルー

		// 頂点vから出ている各辺について探索
		for(auto &e : G[v])
		{
			ll d_next = dist[v] + e.weight;
			if(dist[e.to] == INF64)  // 初めて到達
			{
				dist[e.to] = d_next;
				que.push({dist[e.to], e.to});
			}
			else if(dist[e.to] == d_next)
			{
				;  // チェック済の頂点 矛盾なしのため継続（キューには積まない）
			}
			else
			{
				return true;  // 矛盾あり
			}
		}
	}
	return false;  // 矛盾なし
}

struct UnionFind
{
	vector<int> parent;  // parent[x]:xの親の番号 xが根であれば-1
	vector<int> siz;  // siz[x]:xの属するグループの頂点数

	// 頂点数nで初期化
	UnionFind(int n) : parent(n, -1), siz(n, 1) {}

	// 根を求める
	int root(int x)
	{
		if(parent[x] == -1) return x;  // xが根であれば、xを返す
		else
		{
			parent[x] = root(parent[x]);
			return parent[x];
		}
	}

	// xとyが同じグループに属するか
	bool issame(int x, int y)
	{
		return root(x) == root(y);  // 根が同一なら同じグループ
	}

	// xを含むグループとyを含むグループを併合する
	bool unite(int x, int y)
	{
		// x,yをそれぞれ根まで移動させる
		x = root(x);
		y = root(y);

		// すでに同じグループであれば何もしない
		if(x == y) return false;

		// 頂点数が少ない方が子になるようにする
		if(siz[x] < siz[y])
		{
			// xをyの子にする
			parent[x] = y;
			siz[y] += siz[x];
		}
		else
		{
			// yをxの子にする
			parent[y] = x;
			siz[x] += siz[y];
		}
		return true;
	}

	// xを含むグループの頂点数
	int size(int x)
	{
		return siz[root(x)];
	}
};

int main(void)
{
	int i;
	ll N, M, Q; cin >> N >> M >> Q;
	Graph g(N);
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		ll c;
		cin >> a >> b >> c;  // a->bへ重みc
		a--; b--;
		g[a].push_back({b, c});
		g[b].push_back({a, -c});
		uf.unite(a, b);
	}

	vector<bool> inf(N, false);
	vector<ll> dist(N, INF64);
	for(i = 0; i < N; i++)
	{
		if(i != uf.root(i)) continue;  // 連結成分ごと
		if(dijkstra(g, i, dist))
		{
			inf[i] = true;  // 矛盾発生
		}
	}

	while(Q > 0)
	{
		Q--;
		int x, y; cin >> x >> y;
		x--; y--;
		if(uf.issame(x, y))
		{
			if(inf[uf.root(x)]) 
			{
				cout << "inf" << endl;
			}
			else
			{
				cout << dist[y]-dist[x] << endl;
			}
		}
		else cout << "nan" << endl;
	}

	return 0;
}
