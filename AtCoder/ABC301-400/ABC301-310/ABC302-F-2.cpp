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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC302 https://atcoder.jp/contests/abc302

/*
 * 公式解説を元にした実装をやってみた。
 *   https://atcoder.jp/contests/abc302/editorial/6411
 * 
 * 集合Sを頂点0～(N-1)に、
 * 値Xをまとめる超頂点Xを頂点N～(N+M-1)に、合わせてN+M頂点のグラフで考える。
 * 集合Siに値kが含まれていれば、Siと超頂点k間で辺を張る。
 * 
 * 超頂点1～超頂点Mへの距離が答…だが、求めたいのは集合から集合への距離になるので、
 *   超頂点1～集合a, 集合b～超頂点M への距離は不要  -2する
 *   通常の頂点どうしに距離が2あるので、2で割る
 * という処理が必要。
 * 
 * 自身で考えた手法は1200msほどかかっていたが、こちらは200ms程度と大きく差が出た。
 * 辺数やダイクストラ/BFSなどの違いによるものと思う。
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
	int i, j;
	int N, M; cin >> N >> M;
	Graph g(N+M);
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		for(j = 0; j < a; j++)
		{
			int s; cin >> s;
			s--;
			// 集合i <-> 超頂点s
			g[i].push_back(s+N);
			g[s+N].push_back(i);
		}
	}
	vector<int> dist(N+M, INF32);
	bfs_graph(g, N, dist);  // 超頂点1を起点
	int ans = -1;
	if(dist[N+M-1] != INF32)
	{
		ans = (dist[N+M-1]-2)/2;
	}
	cout << ans << endl;

	return 0;
}
