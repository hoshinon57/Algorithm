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

// ABC257 https://atcoder.jp/contests/abc257

/*
 * 超頂点を用いたBFSで解く。
 * 参考記事：
 *   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2022/0625_abc257
 *   https://x.com/kyopro_friends/status/1540693275677380609
 *   https://zenn.dev/kiwamachan/articles/4be7ade4bd63f7
 * 
 * 小問題ごとに変化するものの、小問題ごとに「テレポータの片側はグラフ全体で固定」となる点に着目する。
 * この町を超頂点として管理する。
 * 
 * i=kの小問題について、頂点0->(N-1)へのルートは3パターンあり、この最小が答となる。
 *   (1)0->N = k->(N-1)
 *   (2)0->k = N->(N-1)
 *   (3)0->(N-1)   テレポータを使わないケース
 * (1)(2)については、頂点0からのBFSと(N-1)からのBFSを行っておくことで求められる。
 * 
 * [ACまでの思考の流れ]
 * ・図を描いていったときに、超頂点と「両側からBFS」が見えた。
 * ・「もう片方が未定であり、各小問題についてそれが一意に決まる」 ⇒ 未定の部分を超頂点でまとめるという考え方。
 */

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
	int N, M; cin >> N >> M;
	Graph g(N+1);  // 0～(N-1)と、超頂点N
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		if(u != -1)
		{
			g[u].push_back(v);
			g[v].push_back(u);
		}
		else
		{
			g[N].push_back(v);
			g[v].push_back(N);
		}
	}
	vector<int> dist(N+1, INF32), dist_r(N+1, INF32);
	bfs_graph(g, 0, dist);
	bfs_graph(g, N-1, dist_r);

	for(i = 0; i < N; i++)  // k=iについて解く
	{
		int ans = INF32;
		int tmp1 = dist[N] + dist_r[i];  // N:超頂点
		int tmp2 = dist_r[N] + dist[i];
		chmin(ans, min(tmp1, tmp2));
		chmin(ans, dist[N-1]);  // テレポータを使わないケース

		if(ans == INF32) ans = -1;
		cout << ans;
		if(i != N-1) cout << " ";
	}
	cout << endl;

	return 0;
}
