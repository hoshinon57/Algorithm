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

// ABC239 https://atcoder.jp/contests/abc239

/*
 * DFSで解く。公式解説と同じ考え方。
 *   https://atcoder.jp/contests/abc239/editorial/3385
 * 
 * 方針としては葉から順に、頂点vの部分木に含まれる整数を、大きい方からMAX_K(=20)個求めていく。
 * ある頂点vについて、その子たち(c1,c2,...とする)の整数が求まっているならば、
 *   X[v]
 *   c1の整数, c2の整数, ...
 * を合算し、大きい方からMAX_K個まで残すという方針で求められる。
 * 
 * MAX_Kは20であるため N*MAX_K=2*10^6 であり、
 * 「大きい方から」の部分にソートを使うことを加味しても十分に間に合う。
 */

using Graph = vector<vector<int>>;
const ll MAX_K = 20;
vector<vector<ll>> num;  // num[i]:頂点iの部分木の整数を大きい方から20個
vector<ll> x;

// 木であるグラフについて、頂点vを(部分木の)根としてDFS
// p:vの親
// 呼び出し元からは dfs(g, 0, -1) や dfs(g, 0) などで呼び出す
vector<ll> dfs_tree(Graph &g, int v, int p = -1)
{
	num[v].push_back(x[v]);  // 自身の値

	// vの子の部分木を再帰で求め、num[v]に入れていく
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		for(auto &ee : dfs_tree(g, e, v))
		{
			num[v].push_back(ee);
		}
	}

	// 大きい方から最大MAX_K個
	sort(num[v].begin(), num[v].end(), greater<ll>());
	if(num[v].size() > MAX_K) num[v].resize(MAX_K);
	
	return num[v];
}

int main(void)
{
	int i;
	ll N, Q; cin >> N >> Q;
	Graph g(N);
	num.resize(N);
	x.resize(N);
	for(i = 0; i < N; i++) {cin >> x[i];}
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	dfs_tree(g, 0);

	while(Q > 0)
	{
		int v, k; cin >> v >> k;
		v--; k--;
		cout << num[v][k] << endl;
		Q--;
	}

	return 0;
}
