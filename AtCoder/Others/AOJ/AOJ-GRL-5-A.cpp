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

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_A

/*
 * [典型]グラフの直径は最短距離計算を2回やる
 * これは辺が重み付きでも同じ。
 * 
 * 重み付きの辺だが、木であるためダイクストラではなくBFSで解くことができる。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

// 頂点sから最も遠い頂点を返す BFS使用
int mostfar(Graph &G, int s, vector<int> &dist)
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
			if(dist[e.to] != INF32) continue;
			dist[e.to] = dist[v] + e.weight;
			que.push(e.to);
		}
	}
	int idx = -1, mx = -1;
	for(int i = 0; i < (int)dist.size(); i++)
	{
		if(chmax(mx, dist[i])) idx = i;
	}
	return idx;
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int s, t, w; cin >> s >> t >> w;
		g[s].push_back({t, w});
		g[t].push_back({s, w});
	}
	vector<int> dist(N, INF32);
	int s = mostfar(g, 0, dist);
	dist.assign(N, INF32);
	int t = mostfar(g, s, dist);
	cout << dist[t] << endl;  // s-t間が直径

	return 0;
}
