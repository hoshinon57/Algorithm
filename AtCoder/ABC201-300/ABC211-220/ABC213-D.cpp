#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC213 https://atcoder.jp/contests/abc213

// 各頂点について、繋がる頂点をソートしておく(制約より)。
// DFSで「行きがけ時」と「子から戻ってきた時」の2つを出力すれば良い。

using Graph = vector<vector<int>>;
Graph g;
vector<int> ans;

void dfs(int v, int p = -1)
{
	ans.push_back(v);
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs(e, v);
		ans.push_back(v);
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	g.resize(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	for(i = 0; i < N; i++)  // 各頂点について、繋がる頂点をソートしておく
	{
		sort(g[i].begin(), g[i].end());
	}

	dfs(0);
	for(auto &e : ans) { cout << e+1 << " "; }
	cout << endl;

	return 0;
}
