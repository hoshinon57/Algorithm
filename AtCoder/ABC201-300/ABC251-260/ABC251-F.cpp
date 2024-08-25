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

// ABC251 https://atcoder.jp/contests/abc251

// グラフをぐっとにらむと、
// T1は頂点1からDFSして探索したルートになるっぽいと分かった。
// ならT2はDFSと対になる(？)BFSにて探索してみるとどうか、とやってみたらビンゴだった。

using Graph = vector<vector<int>>;

void dfs(Graph &g, int v, int p, vector<bool> &seen)
{
	seen[v] = true;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		if(seen[e]) continue;
		cout << v+1 << " " << e+1 << endl;
		dfs(g, e, v, seen);
	}
}
int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	// DFS
	vector<bool> seen;
	seen.assign(N, false);
	dfs(g, 0, -1, seen);

	// BFS
	seen.assign(N, false);
	seen[0] = true;
	queue<int> que;
	que.push(0);
	while(!que.empty())
	{
		auto v = que.front();
		que.pop();
		for(auto &e : g[v])
		{
			if(seen[e]) continue;
			seen[e] = true;
			cout << v+1 << " " << e+1 << endl;
			que.push(e);
		}
	}

	return 0;
}
