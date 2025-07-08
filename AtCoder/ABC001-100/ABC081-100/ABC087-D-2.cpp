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

// ABC087 https://atcoder.jp/contests/abc087

// 各種解説を参考に、DFSやBFSで「矛盾が無いか」を探索。
// 連結成分ごとにやる点に注意

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

bool solve(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		ll l, r, d; cin >> l >> r >> d;
		l--; r--;
		g[l].push_back({(int)r, d});
		g[r].push_back({(int)l, -d});
	}

	vector<ll> w(N, INF64);
	auto dfs = [&](auto &self, int v, ll wei) -> bool
	{
		if(w[v] != INF64)
		{
			return (w[v] == wei);
		}
		w[v] = wei;
		for(auto &e : g[v])  // e:辺
		{
			if(!self(self, e.to, wei+e.weight)) return false;  // 矛盾があればfalseで返していく
		}
		return true;
	};
	for(i = 0; i < N; i++)  // 連結成分ごと
	{
		if(w[i] != INF64) continue;
		if(!dfs(dfs, i, 0))
		{
			return false;
		}
	}
	return true;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
