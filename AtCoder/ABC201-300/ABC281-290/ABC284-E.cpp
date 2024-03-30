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

// ABC284 https://atcoder.jp/contests/abc284

/*
 * DFSで行きがけ時にカウントを加算していく。
 * パスをカウントしたいので、seen[]は帰りがけ時にfalseに戻しておく。
 * 
 * 「次数は10以下」という制約がどう効いてくるのか分からなかったが、解説を見てなるほど…と。
 *   https://atcoder.jp/contests/abc284/editorial/5494
 * 答の上限が10^6なので、逆に言えばdfs()を最低でも10^6だけ呼び出す必要がある。
 * そこから各辺の遷移にて、行き先が全て探索済みの頂点だと、10回ずつ無駄なループが発生する。
 * ⇒10^6*10=10^7で間に合う。
 * これが「次数1000以下」だと、ワーストで10^6*1000=10^9回の処理となり、TLEしてしまう。
 */

using Graph = vector<vector<int>>;
const ll BRD = 1e6;
vector<bool> seen;
ll cnt;

// 見つけたパスが10^6を超えたらtrueを返して終了
bool dfs(Graph &g, int v)
{
	if(seen[v]) return false;
	seen[v] = true;

	cnt++;
	if(cnt > BRD) return true;

	for(auto &e : g[v])
	{
		if(dfs(g, e)) return true;
	}

	seen[v] = false;
	return false;
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
	seen.resize(N, false);
	dfs(g, 0);
	cout << min(cnt, BRD) << endl;

	return 0;
}
