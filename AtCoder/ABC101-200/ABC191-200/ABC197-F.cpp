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

// ABC197 https://atcoder.jp/contests/abc197

/*
 * 大まかな方針はあっていたが、考察の一部がまずくafter_contestでやられた。
 * BFSで解く。
 * 
 * まず頂点1とNにコマを置き、同時に移動させる。
 * このとき同じ文字であるような辺を選ぶようにする。
 * dist[(コマ1の頂点)][(コマ2の頂点)]として移動回数を保持すれば、後はいつものBFSになる。
 * ABC289-E(Swap Places)が近い。
 * 
 * BFSの後、出力すべき解は長さが偶数/奇数でそれぞれ計算して小さい方を採用する。
 * 偶数：
 *   操作途中でコマ1,2が同じマスに来るので、dist[i][i]*2の最小値。
 * 奇数：
 *   操作途中でコマ1,2が隣マスに来るので、
 *   各頂点iについて隣の頂点vを列挙し、dist[i][v]*2+1の最小値。
 * ＃反省点
 * 最初、dist[N-1][0]が答になると考えてしまい、after_contest_00.txtでやられた。考察ミス。
 * ---------
 * 4 4
 * 1 2 a
 * 2 4 b
 * 1 3 b
 * 3 4 a
 * ---------
 * の出力が-1であるべきが、2を出力してしまっていた
 * 
 * [ACまでの思考の流れ]
 * ・回文は前後から見るとうまく行くケースがある。
 */

struct Edge
{
	int to;  // 隣接頂点番号
	char c;
	Edge(int to_, char c_) : to(to_), c(c_) {}
};
using Graph = vector<vector<Edge>>;

void bfs_graph(Graph &g, int s, int t, vector<vector<int>> &dist)
{
	queue<pair<int,int>> que;  // 2頂点{u,v}
	dist[s][t] = 0;
	que.push({s, t});

	while(!que.empty())
	{
		auto [u,v] = que.front();
		que.pop();
		for(auto &uu : g[u])
		{
			for(auto &vv : g[v])
			{
				if(uu.c != vv.c) continue;
				if(dist[uu.to][vv.to] != INF32) continue;  // 探索済み
				dist[uu.to][vv.to] = dist[u][v] + 1;
				que.push({uu.to, vv.to});
			}
		}
	}
	return;
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		char c; cin >> c;
		g[a].push_back({b, c});
		g[b].push_back({a, c});
	}

	vector<vector<int>> dist(N, vector<int>(N, INF32));  // dist[N][N]
	bfs_graph(g, 0, N-1, dist);
	int ans = INF32;
	for(i = 0; i < N; i++)  // 長さ偶数
	{
		chmin(ans, dist[i][i]*2);
	}
	for(i = 0; i < N; i++)  // 長さ奇数
	{
		for(auto &v : g[i])
		{
			chmin(ans, dist[i][v.to]*2+1);
		}
	}
	if(ans == INF32) ans = -1;
	cout << ans << endl;
#if 0  // 最初に考えた間違い考察。after_contestで落ちる。
	int ans = dist[N-1][0];
	if(ans == INF32) ans = -1;
	cout << ans << endl;
#endif

	return 0;
}
