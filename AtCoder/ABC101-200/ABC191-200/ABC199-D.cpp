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

// ABC199 https://atcoder.jp/contests/abc199

/*
 * 自力で解けず、解説を見た。
 * 深さ優先探索で解く。
 *   https://atcoder.jp/contests/abc199/editorial/1163
 *   https://blog.hamayanhamayan.com/entry/2021/04/25/002321
 *   https://twitter.com/kyopro_friends/status/1385954017562398727
 * 
 * 基本は上記解説の通り。
 * ポイントは、3^NならTLEだが2^Nなら間に合うことかなと。
 * DFSを2種類用意し、
 * 1種類目：
 *   連結成分ごとに、先頭の頂点からたどって行きがけ順に頂点をリストアップする。
 * 2種類目：
 *   先のリストの順に色を塗っていく。
 *   リストの先頭を除き、ある頂点vを見ているときに塗り済みの頂点は必ず1つはあるので、
 *   塗り方は高々2パターンとなる。 ⇒ これが2^Nの根拠。
 *   先頭は赤固定とし、最後に3倍すればよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・3^NはTLEだが2^Nなら間に合いそう、という考え方。
 * ・ある頂点について塗り方が3パターンいずれも可能なのは、連結成分ごとに1つ、つまり最初の頂点ぐらい。
 *   基本は2パターン以下しか無い。
 *   なら先頭から塗り方を全探索しても間に合いそう。
 */

using Graph = vector<vector<int>>;

// DFS1種類目
void dfs(int v, Graph &g, vector<bool> &seen, vector<int> &vlist)
{
	seen[v] = true;
	vlist.push_back(v);
	for(auto &e : g[v])
	{
		if(seen[e]) continue;
		dfs(e, g, seen, vlist);
	}
}

// DFS2種類目
// vlistに対して先頭から色を塗っていく
ll dfs_2(int idx, Graph &g, vector<int> &col, vector<int> &vlist)
{
	int v = vlist[idx];
	vector<bool> c_used(3, false);  // [i]:色iが隣の頂点で使われていたらtrue
	if(idx == 0)  // 先頭は赤固定
	{
		c_used[1] = c_used[2] = true;
	}
	else if(idx == (int)vlist.size()) { return 1; } // 最後まで塗れた
	else
	{
		for(auto &e : g[v])  // 隣接頂点で使っている色をチェック
		{
			if(col[e] == -1) continue;
			c_used[col[e]] = true;
		}
	}

	ll cnt = 0;
	for(int i = 0; i < 3; i++)  // まだ使っていない色で塗ってDFS
	{
		if(c_used[i]) continue;
		col[v] = i;
		cnt += dfs_2(idx+1, g, col, vlist);
		col[v] = -1;
	}
	return cnt;
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b; a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	ll ans = 1;
	vector<bool> seen(N, false);
	for(i = 0; i < N; i++)
	{
		vector<int> vlist;
		if(seen[i]) continue;
		dfs(i, g, seen, vlist);

		vector<int> col(N, -1);  // col[i]:頂点iに設定する色
		ll cnt = dfs_2(0, g, col, vlist) * 3;  // 先頭を赤固定とするので、3倍
		ans *= cnt;  // 連結成分ごとの積
	}
	cout << ans << endl;

	return 0;
}
