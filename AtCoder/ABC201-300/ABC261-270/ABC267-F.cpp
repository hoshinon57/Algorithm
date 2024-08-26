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

// ABC267 https://atcoder.jp/contests/abc267

/*
 * 自力で解けず、解説を見た。
 *   https://atcoder.jp/contests/abc267/editorial/4714
 * 
 * 基本は公式解説の通り。
 * 与えられた木の直径となる2頂点をS,Tとすると、
 * 任意の頂点uについて u->S もしくは u->T のどちらかが必ず(uを起点とした)最長パスとなる。
 * よってuから距離kの頂点は、上記2つのパス上のどちらかに存在するか、もしくは存在しない。
 * 
 * よって以下の手順で解ける。
 * ・クエリ先読みをし、no番目のクエリに{u,k}に対して que[u] = {{k,no}, ...} としておく。
 * ・木の直径を求める。
 * ・S,Tそれぞれを根とした部分木について、DFSを行う。
 *   行きがけ時に頂点番号をpath[]に入れていき、
 *   vがクエリに含まれるのであれば、path[]を用いて答を探索する。
 *   ※ansは-1で初期化しておく
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・木の直径となる2頂点をS,Tとすると、
 *   任意の頂点uについて u->S もしくは u->T のどちらかが必ず(uを起点とした)最長パスとなる。
 * ・上記の知識から、S,Tを部分木としたDFSを用いることで、
 *   任意の頂点から距離kとなる頂点を1つ求めることができる。
 */

using Graph = vector<vector<int>>;
vector<vector<pair<int,int>>> que;  // no番目のクエリ{u,k}に対して que[u] = {{k, no}, ...}   
vector<int> ans;  // ans[i]:クエリiに対する答
vector<int> path;

// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist)
{
	queue<int> que_;
	dist[s] = 0;
	que_.push(s);

	while(!que_.empty())
	{
		int v = que_.front();
		que_.pop();
		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que_.push(e);
		}
	}
	return;
}

// 頂点sから最も遠い頂点番号を返す
int tree_most_far(Graph &g, int s)
{
	vector<int> dist(g.size(), INF32);
	bfs_graph(g, s, dist);
	int mx = -1, ret;
	for(int i = 0; i < (int)dist.size(); i++)
	{
		if(chmax(mx, dist[i])) ret = i;
	}
	return ret;
}

// 頂点vからDFS
// 行きがけ時にpathに頂点番号を入れ、クエリ頂点があればpath[]からkだけさかのぼった頂点をans[]へ設定する
void dfs(Graph &g, int v, int p = -1)
{
	path.push_back(v);
	for(auto [k, no] : que[v])  // 頂点vに対するクエリ
	{
		// path[]からk個さかのぼった頂点、があればそれが答
		int idx = (int)path.size()-1-k;
		if(idx >= 0) ans[no] = path[idx]+1;
	}
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs(g, e, v);
	}
	path.pop_back();
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
	int Q; cin >> Q;
	que.resize(N);
	for(i = 0; i < Q; i++)
	{
		int u, k; cin >> u >> k;
		u--;
		que[u].push_back({k, i});
	}

	// 木の直径 s->t を求める
	int s = tree_most_far(g, 0);
	int t = tree_most_far(g, s);

	// s,tそれぞれを根としてDFS
	ans.resize(Q, -1);
	path.clear();
	dfs(g, s);
	path.clear();
	dfs(g, t);

	for(auto &e : ans) cout << e << endl;

	return 0;
}
