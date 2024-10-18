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

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_B

/*
 * 木の直径をs-tとすると、
 * 各頂点iについて i-s もしくは i-t が最も遠い点となる。
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
	int idx = max_element(dist.begin(), dist.end()) - dist.begin();
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
	vector<int> dist_0(N, INF32), dist_s(N, INF32), dist_t(N, INF32);
	int s = mostfar(g, 0, dist_0);
	int t = mostfar(g, s, dist_s);
	mostfar(g, t, dist_t);
	for(i = 0; i < N; i++)
	{
		cout << max(dist_s[i], dist_t[i]) << endl;
	}

	return 0;
}
