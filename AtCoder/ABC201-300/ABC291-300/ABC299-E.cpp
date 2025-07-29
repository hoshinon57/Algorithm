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

// 以前は2023/4時の実装だった。
// 2025/7に再解きしたものに置き換える。

// まず全頂点を黒
// 各p,dでBFSし、頂点pから距離d未満の頂点を白に変更
// 再度、p,dでBFSし、現在の色で条件を満たすか判定

using Graph = vector<vector<int>>;

// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist)
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
	int i, j;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	int K; cin >> K;
	vector<int> p(K), d(K); for(i = 0; i < K; i++) {cin >> p[i] >> d[i]; p[i]--; }

	vector<int> col(N, 1);  // col[i]: 頂点iの色 1:黒, 0:白
	vector<int> dist(N);
	for(i = 0; i < K; i++)
	{
		for(auto &e : dist) e = INF32;
		bfs_graph(g, p[i], dist);
		for(j = 0; j < N; j++)
		{
			if(dist[j] < d[i]) col[j] = 0;
		}
	}

	for(i = 0; i < K; i++)
	{
		for(auto &e : dist) e = INF32;
		bfs_graph(g, p[i], dist);
		bool flg = false;
		for(j = 0; j < N; j++)
		{
			if(dist[j] == d[i] && col[j] == 1)
			{
				flg = true;
			}
		}
		if(!flg)
		{
			cout << "No" << endl;
			return 0;
		}
	}

	cout << "Yes" << endl;
	for(auto &e : col) cout << e;
	cout << endl;

	return 0;
}
