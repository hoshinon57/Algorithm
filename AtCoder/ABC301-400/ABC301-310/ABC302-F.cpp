#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC302 https://atcoder.jp/contests/abc302

/*
 * 超頂点を用いたダイクストラ法で解く。
 * ほぼ公式解説と同じ考え方だった。
 *   https://twitter.com/kyopro_friends/status/1659921415444000768
 *   https://atcoder.jp/contests/abc302/editorial/6411
 * 
 * まず、Si,jに対応する頂点番号を用意する関数を作る。
 * これはpair<int,int> -> int への連想配列を用いた。
 * 実装はtoID()を参照。すでに作成済みならばそれを、未作成なら新規に作成する処理としている。
 * 
 * 例えば入力例1において、S1={1,2}, S2={2,3}であり、
 * S1,1とS1,2間、およびS2,1とS2,2間にそれぞれコスト0の辺を張る。
 * また値が同じ2であるS1,2とS2,1にはコスト1の辺を張る。
 * このコスト1が操作回数に相当する。
 * 値が1である(いずれかの)頂点から、値がMである(いずれかの)頂点への最小コストを求める問題となる。
 * 
 * ただし各頂点間に辺を張ると辺の数が膨大になるため、超頂点を用意する。
 *   (1)値がXである頂点
 *   (2)超頂点X
 * について、(1)->(2)へはコスト1, (2)->(1)へはコスト0の辺を張る。
 * これで辺の数を抑えられる。
 * ※toID()では(-1,X)として引数を指定している
 * 超頂点1から超頂点Mへのコストが答となる。
 * ただし実際の操作回数としては超頂点Mへのコスト1は不要なので、-1したものが答。
 * 
 * 後は上記のように丁寧にグラフを構築し、ダイクストラのライブラリに入れれば完了。
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
void dijkstra(Graph &G, int s, vector<ll> &dist)
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
			ll d_next;
			// 頂点vと辺eを用いた先の頂点について、距離を求める。
			// 基本はdist[v]+e.weightで良いが、問題によっては＋αが必要。(ABC192-E,ABC204-E等)
			d_next = dist[v] + e.weight;
			if(dist[e.to] > d_next)
			{
				dist[e.to] = d_next;
				que.push({dist[e.to], e.to});
			}
		}
	}
}

map<pair<int,int>,int> mp;  // mp[{i,j}]:(i,j)に対応する頂点番号 未取得なら新規作成
int mp_cnt = 0;  // 作成済みの頂点数
// Si,jに対応する頂点番号を返す
// 作成済みならそれを返し、未作成なら新規作成する
int toID(int i, int j)
{
	if(mp.count({i,j}) == 0)
	{
		mp[{i,j}] = mp_cnt++;
	}
	return mp[{i,j}];
}

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	Graph g;
	// 超頂点(-1,X)を先に作る
	for(i = 1; i <= M; i++)
	{
		int id = toID(-1, i);
		(void)id;
		g.push_back({});
	}
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		int prev = -1;
		for(j = 0; j < a; j++)
		{
			int s; cin >> s;  // {i,s}
			int id = toID(i, s);
			g.push_back({});
			if(prev != -1)
			{
				// Si,jとSi,j-1 間の辺を張る
				int prev_id = toID(i, prev);
				g[id].push_back({prev_id, 0});
				g[prev_id].push_back({id, 0});
			}
			prev = s;
			// 超頂点
			int cho_id = toID(-1, s);
			g[id].push_back({cho_id, 1});  // 超頂点へはコスト1
			g[cho_id].push_back({id, 0});  // 超頂点からはコスト0
		}
	}

	// 超頂点1から超頂点Mまでのコストが答
	int st = toID(-1, 1);
	int ed = toID(-1, M);
	vector<ll> dist(g.size(), INF64);
	dijkstra(g, st, dist);
	ll ans = -1;
	if(dist[ed] != INF64) ans = dist[ed]-1;  // 値がMとなる頂点から超頂点Mへのコストは、実際の操作回数としては不要なので-1する
	cout << ans << endl;

	return 0;
}
