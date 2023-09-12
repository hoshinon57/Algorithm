#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC202 https://atcoder.jp/contests/abc202

/*
 * 自力で解けず、解説を見た。
 * 深さ優先探索で解く。おそらくオイラーツアー的な感じ。
 * https://atcoder.jp/contests/abc202/editorial/1864
 * 
 * 根からDFSを行い、行きがけ順/帰りがけ順を合わせた探索順を、それぞれvin[v],vout[v]に保持する。
 * また、depth[d]には深さdとなる頂点の行きがけ時の探索順を入れていく。
 * 
 * すると、ある親子2頂点について以下が成り立つ。
 *   vin[親] <= vin[子] < vout[親]
 * vin[v]～vout[v]で囲まれた範囲に、頂点vを根とした部分木に含まれる頂点が含まれるイメージ。
 * depth[d]にも同等の情報を入れているため、
 * クエリごとにdepth[d]のうち vin[u]～vout[u] の値をlower_bound()で求めればよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・部分木と言えばオイラーツアー。
 *   https://blog.hamayanhamayan.com/entry/2021/05/22/225525
 * ・DFSにて訪れた順に頂点を並べると、部分木の頂点ごとに連続している(典型)。
 *   https://kanpurin.hatenablog.com/entry/2021/05/22/231444
 */

using Graph = vector<vector<int>>;

int cnt;  // cnt:DFSでの探索順 各頂点ごとに、行きがけ時に+1, 帰りがけ時にも+1する
vector<int> vin, vout;  // 行きがけ時/帰りがけ時における探索順
vector<vector<int>> depth;  // depth[d]:深さdである頂点について、行きがけ時の探索順を入れていく

// p:vの親
// d:vの深さ
void dfs_tree(Graph &g, int v, int p = -1, int d = 0)
{
	depth[d].push_back(cnt);
	vin[v] = cnt++;
	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		dfs_tree(g, e, v, d+1);
	}
	vout[v] = cnt++;
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 1; i < N; i++)
	{
		int p; cin >> p;
		p--;
		g[i].push_back(p);
		g[p].push_back(i);
	}

	vin.resize(N);
	vout.resize(N);
	depth.resize(N);
	dfs_tree(g, 0);

	int Q; cin >> Q;
	while(Q > 0)
	{
		int u, d; cin >> u >> d;
		u--;

		// depth[d]について、vin[u]～vout[u]の個数
		int ans = lower_bound(depth[d].begin(), depth[d].end(), vout[u]) - lower_bound(depth[d].begin(), depth[d].end(), vin[u]);
		cout << ans << endl;

		Q--;
	}

	return 0;
}
