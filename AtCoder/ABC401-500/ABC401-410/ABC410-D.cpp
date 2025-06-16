#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC410 https://atcoder.jp/contests/abc410

/*
 * {頂点番号, そこに来るまでの辺xor}を持って到達判定。
 * 到達判定なのでDFS/BFSどちらでも可能。
**/

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

vector<vector<bool>> seen;
void dfs(Graph &g, int v, int w)
{
	if(seen[v][w]) return;
	seen[v][w] = true;

	for(auto &e : g[v])  // e:辺
	{
		dfs(g, e.to, w^e.weight);
	}
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
		int w; cin >> w;
		g[a].push_back({b, w});
	}
	seen.resize(N, vector<bool>(1024+10, false));
	dfs(g, 0, 0);

	int ans = INF32;
	for(i = 0; i < 1024; i++)
	{
		if(seen[N-1][i])
		{
			chmin(ans, i);  // xor=iで到達可能
		}
	}
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
