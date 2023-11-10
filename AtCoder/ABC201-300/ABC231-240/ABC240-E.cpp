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

// ABC240 https://atcoder.jp/contests/abc240

/*
 * 問題文の理解が難しかったが、結局は
 * ・根からDFS
 * ・葉にはL=Rとなる値を1から順に振っていく
 * ・葉でない要素には、L=(子のLの最小値), R=(子のRの最大値) とする
 * で解ける。
 */

using Graph = vector<vector<int>>;
int cnt = 1;
vector<int> l, r;

void dfs_tree(Graph &g, int v, int p = -1)
{
	if(g[v].size() == 1 && v != 0)  // 葉
	{
		l[v] = r[v] = cnt;
		cnt++;
		return;	
	}

	for(auto &e : g[v])
	{
		if(e == p) continue;  // 親への逆流を禁止
		dfs_tree(g, e, v);
		l[v] = min(l[v], l[e]);
		r[v] = max(r[v], r[e]);
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	l.resize(N, INF32);
	r.resize(N, 0);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	dfs_tree(g, 0);

	for(i = 0; i < N; i++)
	{
		cout << l[i] << " " << r[i] << endl;
	}

	return 0;
}
