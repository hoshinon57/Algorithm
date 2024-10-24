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

// ABC376 https://atcoder.jp/contests/abc376

// BFSの自作ライブラリを少し変えて、
// 行き先に頂点0があり、dist[0]=0のときだけ設定する、という特殊処理を入れると上手くいく。

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
			// 本問用の特殊処理
			if(e == 0)
			{
				if(dist[e] != 0) continue;
				dist[e] = dist[v] + 1;
				// not push
			}
			else
			{
				if(dist[e] != INF32) continue;
				dist[e] = dist[v] + 1;
				que.push(e);
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
		g[a].push_back(b);
	}
	vector<int> dist(N, INF32);
	bfs_graph(g, 0, dist);

	int ans = dist[0];
	if(ans == 0) ans = -1;
	cout << ans << endl;

	return 0;
}
