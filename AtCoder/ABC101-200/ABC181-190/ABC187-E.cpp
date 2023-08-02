#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC187 https://atcoder.jp/contests/abc187

/*
 * 頂点1を根として考える。
 * クエリはa,bどちらが根に近いかによって、以下2パターンとなる。
 * ある頂点vを根とした部分木のうち、
 * (1)その部分木の頂点全てにxを足す
 * (2)その部分木を除く頂点全てにxを足す
 * 
 * offset[N]を用意して、v以下の全頂点にoffsetを加算することを考えると、
 * (1)は offset[v]+=x とするだけでよい。
 * (2)は offset[0]+=x, offset[v]-=x とすればよい。
 * 
 * クエリのa,bどちらが根に近いかは、事前にDFSで各頂点の深さを求めておく。
 * 実装ではdfs_depth()に相当する。
 * 
 * ※以下の解説と同じ考え方。
 *   https://twitter.com/kyopro_friends/status/1345365869929459713
 *   https://drken1215.hatenablog.com/entry/2021/01/03/162800
 */

using Graph = vector<vector<int>>;
vector<int> depth;  // 各頂点の根からの深さ (根を0とする)
vector<ll> offset;
vector<ll> ans;

void dfs_depth(Graph &g, int v, int d = 0, int p = -1)
{
	depth[v] = d;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs_depth(g, e, d+1, v);
	}
}

void dfs_calc(Graph &g, int v, ll num = 0, int p = -1)
{
	ans[v] = offset[v] + num;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs_calc(g, e, ans[v], v);
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N; cin >> N;
	Graph g(N);
	vector<pair<int,int>> edge;  // edge[i]:i番目の辺をつなぐ2頂点{a,b}
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
		edge.push_back({a, b});
	}
	depth.resize(N);
	dfs_depth(g, 0);

	offset.resize(N);
	ans.resize(N);
	int Q; cin >> Q;
	while(Q > 0)
	{
		ll t, e, x; cin >> t >> e >> x;
		e--; 

		// a側を+xするように調整
		int a_, b_;
		a_ = edge[e].first;
		b_ = edge[e].second;
		if(t == 2) swap(a_, b_);

		if(depth[a_] < depth[b_])  // aの方が根に近い
		{
			// bの部分木を除いたものにxを加算
			offset[0] += x;
			offset[b_] -= x;
		}
		else
		{
			// aの部分木にxを加算
			offset[a_] += x;
		}

		Q--;
	}

	dfs_calc(g, 0);
	for(auto &e : ans)
	{
		cout << e << endl;
	}

	return 0;
}
