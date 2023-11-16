#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC146 https://atcoder.jp/contests/abc146

/*
 * DFSおよび貪欲的な考え方で解く。
 * 
 * 各頂点について、使える色の辺を順に割り当てていけばよい。
 * よって根からDFSしつつ順に塗っていく。
 * 
 * DFSの引数として、親頂点を表すpに加え、親と結んだ辺の色pcolも入れておく。
 * すると頂点vについて色を塗っていくときに、
 * 色を1つずつインクリメントしつつ、pcolは飛ばすという実装で実現できる。
 * 
 * 辺ごとに使う色を出力するにあたり、連想配列を利用した。
 * 
 * [ACまでの思考の流れ]
 * ・どういう手順で塗れば色の数が最小になるか、手を動かしながら考えた。
 * ・どうも根から順に貪欲で行けるっぽい。
 * ・木なのでDFSで実装できる。
 *   親と結ぶ辺の色も引数で与えれば、実装がやりやすそう。
 */

using Graph = vector<vector<int>>;
vector<int> a, b;  // a[i],b[i]:i番目の辺の頂点
map<pair<int,int>, int> col;  // [{u,v}] の2頂点を結ぶ辺の色
int max_col = 0;

// 頂点vを元にDFS.
// p:親の頂点, pcol:v,pを結ぶ辺の色
void dfs_tree(Graph &g, int v, int p = -1, int pcol = -1)
{
	int now = 1;
	if(now == pcol) now++;

	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止

		col[{e, v}] = col[{v, e}] = now;
		dfs_tree(g, e, v, now);
		max_col = max(max_col, now);
		if(++now == pcol) now++;  // 親の色を避けつつ、次の色を選択
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	a.resize(N-1); b.resize(N-1);
	for(i = 0; i < N-1; i++)
	{
		cin >> a[i] >> b[i];
		a[i]--; b[i]--;
		g[a[i]].push_back(b[i]);
		g[b[i]].push_back(a[i]);
	}
	dfs_tree(g, 0);

	cout << max_col << endl;
	for(i = 0; i < N-1; i++)
	{
		cout << col[{a[i], b[i]}] << endl;
	}

	return 0;
}
