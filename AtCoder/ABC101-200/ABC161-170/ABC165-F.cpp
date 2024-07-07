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

// ABC165 https://atcoder.jp/contests/abc165

/*
 * LISを各頂点ごとに求めつつ、木上のDFSにて巻き戻しを使いながら探索していく。
 * 各種解説の通り。
 *   https://img.atcoder.jp/abc165/editorial.pdf
 *   https://x.com/kyopro_friends/status/1256584894345441281
 *   https://motsu-xe.hatenablog.com/entry/2020/05/02/225006
 *   https://qiita.com/python_walker/items/d1e2be789f6e7a0851e5  (LISの求め方参考)
 * 
 * まず、ある数列におけるLISはlower_bound()を用いてO(logN)で求められる。
 * 頂点1からDFSにて入力aを追加しつつ、頂点ごとにLISを求めていく。
 * 
 * 木なので頂点を行ったり来たり、つまりaの入力先の配列が増えたり減ったりするのが問題。
 * 頂点ごとにこの配列は1か所しか変化しないため、
 * どこが変化したかをバックアップし、DFSの帰りがけ時に復元させれば解決する。
 */

using Graph = vector<vector<int>>;
vector<int> a, ans, lis;

void dfs(Graph &g, int v, int p = -1)
{
	// LISのバックアップ＆書き換え＆頂点vにおけるansの計算
	int idx = lower_bound(lis.begin(), lis.end(), a[v]) - lis.begin();
	pair<int,int> bk = {idx, lis[idx]};
	lis[idx] = a[v];
	ans[v] = lower_bound(lis.begin(), lis.end(), INF32) - lis.begin();

	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs(g, e, v);
	}

	// 元に戻す
	lis[bk.first] = bk.second;
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	a.resize(N);
	for(i = 0; i < N; i++) cin >> a[i];
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	ans.resize(N);
	lis.resize(N, INF32);
	dfs(g, 0);
	for(i = 0; i < N; i++) cout << ans[i] << endl;

	return 0;
}
