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

// ABC282 https://atcoder.jp/contests/abc282

/*
 * 二部グラフにて解く。基本は各種解説の通り。
 *   https://atcoder.jp/contests/abc282/editorial/5397
 *   https://x.com/kyopro_friends/status/1604113410983342080
 * 
 * まず、そもそもが二部グラフで無ければ0出力して終了。
 * 
 * 連結成分ごとに{0,1},{2,3},{4,5},... のように色を塗っていく。
 * すると(u,v)として条件を満たさないのは
 *   u,vが同じ色で塗られた
 *   u,vが元々辺で繋がっている
 * の2点。
 * (別の連結成分どうしの頂点なら、条件を常に満たす)
 * これを頂点の全ペア C(N,2) から引けばよい。
 */

using Graph = vector<vector<int>>;

// 頂点vを起点にDFSし、二部グラフにできればtrueを返す
// col[i]:頂点iの色 未探索の場合は-1 呼び出し元で-1で初期化しておくこと
// cur:
//   vに塗る色
//   基本0で呼び出して良いが、他の"偶数"cを指定すると c,c+1 で塗る
//   例：cur=2なら2,3, cur=4なら4,5
// 呼び出し元からは、未探索の頂点vについて(※)
//   (g, v, col)
// のように呼び出していくこと。 (※)col[v]が-1かどうかで判定できる
// 
// ABC282-D用にcnを追加した
bool dfs_bipartite(Graph &g, int v, vector<int> &col, vector<ll> &cn, int cur = 0)
{
	col[v] = cur;
	cn[cur]++;
	for(auto &e : g[v])
	{
		if(col[e] != -1)  // もう塗られている
		{
			if(col[e] == col[v]) return false;
		}
		else  // まだ塗っていない
		{
			int nxt = cur^1;
			if(!dfs_bipartite(g, e, col, cn, nxt)) return false;
		}
	}
	return true;
}

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	vector<int> col(N, -1);
	vector<ll> cn(2*N+1);  // cn[i]:色iで塗られた頂点数 孤立頂点もあるため要素数はN*2必要
	int c = 0;
	for(i = 0; i < N; i++)
	{
		if(col[i] != -1) continue;  // 探索済み
		if(!dfs_bipartite(g, i, col, cn, c))
		{
			cout << 0 << endl;  // そもそもが二部グラフではない
			return 0;
		}
		c+=2;
	}

	ll ans = N*(N-1)/2;  // 初期値は全ペア数
	for(auto &e : cn) ans -= e*(e-1)/2;  // 同じ色で塗られた頂点同士は結べない
	ans -= M;  // 元々辺で繋がっていない、という条件
	cout << ans << endl;

	return 0;
}
