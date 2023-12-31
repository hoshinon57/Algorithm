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

// ABC327 https://atcoder.jp/contests/abc327

// グラフが二部グラフになっていればOK.
// dfs内の再帰にて戻り値チェックするのを忘れていて1WA.

using Graph = vector<vector<int>>;

// 頂点vをnowで塗る
bool dfs(Graph &g, int v, vector<int> &col, int now = 0)
{
	col[v] = now;
	for(auto &e : g[v])
	{
		if(col[e] == col[v]) return false;
		if(col[e] != INF32) continue;
		if(!dfs(g, e, col, 1-now)) return false;
	}
	return true;
}

bool solve(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<int> a(M); for(i = 0; i < M; i++) {cin >> a[i]; a[i]--; }
	vector<int> b(M); for(i = 0; i < M; i++) {cin >> b[i]; b[i]--; }
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		g[a[i]].push_back(b[i]);
		g[b[i]].push_back(a[i]);
	}
	vector<int> col(N, INF32);
	bool ok = true;
	for(i = 0; i < N; i++)
	{
		if(col[i] != INF32) continue;
		if(!dfs(g, i, col)) ok = false;
	}
	return ok;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
