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

// ABC067 https://atcoder.jp/contests/abc067

// 頂点1とNからそれぞれBFSして距離計算し、d1[i]<=d2[i]であれば先手が取れるマス。
// 先手が取れるマス数をcntとすると、後手が取れるマス数は(N-cnt)なので、
// cnt > (N-cnt)なら先手勝ち。

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
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	vector<int> d1(N, INF32), d2(N, INF32);
	bfs_graph(g, 0, d1);
	bfs_graph(g, N-1, d2);

	int cnt = 0;
	for(i = 0; i < N; i++)
	{
		if(d1[i] <= d2[i]) cnt++;
	}
	if(cnt > (N-cnt)) cout << "Fennec" << endl;
	else cout << "Snuke" << endl;

	return 0;
}
