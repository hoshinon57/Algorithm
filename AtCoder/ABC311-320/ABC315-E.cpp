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

// ABC315 https://atcoder.jp/contests/abc315

/*
 * コンテスト中に解けず、解説を見た。大反省。
 * 
 * 入力についてi->Piと逆方向に辺を張ったグラフを考える。
 * 頂点1からDFSし、帰りがけ順に頂点を記録していったものが答。
 */

using Graph = vector<vector<int>>;
vector<int> ans;
vector<bool> seen;

void dfs(Graph &g, int v)
{
	if(seen[v]) return;
	seen[v] = true;
	for(auto &e : g[v])
	{
		dfs(g, e);
	}
	if(v != 0) ans.push_back(v+1);
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N; i++)
	{
		int c; cin >> c;
		for(j = 0; j < c; j++)
		{
			int p; cin >> p;
			p--;
			g[i].push_back(p);  // i->p
		}
	}

	seen.resize(N, false);
	dfs(g, 0);

	int S = (int)ans.size();
	for(i = 0; i < S; i++)
	{
		cout << ans[i];
		if(i != S-1) cout << " ";
	}
	cout << endl;

	return 0;
}
