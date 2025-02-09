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

// ABC070 https://atcoder.jp/contests/abc070

// (x->K)+(y->K)が答。
// Kが固定なので、Kを始点とした最短経路探索を1回やればOK.
// ダイクストラ法のライブラリを張っても良いが、木なのでDFS/BFSとかでもできるので、実装練習としてDFSにした。

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

vector<ll> dep;

void dfs(Graph &g, ll di, int v, int p = -1)
{
	dep[v] = di;
	for(auto &e : g[v])
	{
		if(e.to == p) continue;
		dfs(g, di+e.weight, e.to, v);
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b, c; cin >> a >> b >> c;
		a--; b--;
		g[a].push_back({b, c});
		g[b].push_back({a, c});
	}
	dep.resize(N);

	int Q, K; cin >> Q >> K;
	K--;
	dfs(g, 0, K);
	while(Q > 0)
	{
		Q--;
		int x, y; cin >> x >> y;
		x--; y--;
		ll ans = dep[x] + dep[y];
		cout << ans << endl;
	}

	return 0;
}
