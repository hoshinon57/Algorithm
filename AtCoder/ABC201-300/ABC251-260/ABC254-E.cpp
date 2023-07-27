#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC254 https://atcoder.jp/contests/abc254

/*
 * 幅優先探索で解く。
 * 
 * BFSにて、始点からの距離がkになったら探索打ち切りとする。
 * 次数が3以下であるため、クエリごとにBFSを行っても十分に間に合う。
 * 
 * ただし懸念だったのが、各頂点の距離を保持するdist[N]を毎回初期化すると
 * 初期化処理でO(NQ)かかってしまうため、TLEになりそうだと考えた。
 * そのため、BFSにて探索した頂点をsetで保持しておき、
 * BFS終了後に書き換えた要素のみINF32を設定しなおす、という処理とした。
 */

using Graph = vector<vector<int>>;
// 頂点数Nのグラフについて、頂点sを起点にBFS. ただし距離はk以内。
// 探索した頂点番号の総和を返す。
// 
// distは要素数Nで、未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
int bfs_graph(Graph &G, int N, int s, int k, vector<int> &dist)
{
	int ans = 0;
	queue<int> que;
	dist[s] = 0;
	que.push(s);
	set<int> v_list;  // sからの距離がk以下である頂点の集合

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		v_list.insert(v);
		if(dist[v] == k) continue;  // 探索打ち切り

		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}

	for(auto &e : v_list)
	{
		ans += (e+1);
		dist[e] = INF32;  // N要素全てを毎回初期化するとTLEになりそうなので、変えた部分のみ戻す
	}

	return ans;
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
		g[b].push_back(a);
	}

	int Q; cin >> Q;
	vector<int> dist(N, INF32);
	while(Q > 0)
	{
		int x, k; cin >> x >> k;
		x--;
		cout << bfs_graph(g, N, x, k, dist) << endl;
		Q--;
	}

	return 0;
}
