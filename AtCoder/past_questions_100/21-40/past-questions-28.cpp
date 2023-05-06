#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 「分野別 初中級者が解くべき過去問精選 100問」の問題28
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_C&lang=ja

/*
 * 幅優先探索(BFS)で解く。
 * 
 * 以下、0-indexedで記載する。
 * 頂点0を起点にBFSを行い、頂点0からの距離を計算していく。
 * (いわゆる「教科書通りの実装」であり、説明は省略する)
 * 
 * グラフでのBFSの計算量はO(頂点数+辺数)であることから、
 * 計算量はO(n+Σk)かと考える。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	// 0-indexed
	int i;
	int n;
	cin >> n;
	Graph g(n);
	for(i = 0; i < n; i++)
	{
		int u, k, v;
		cin >> u >> k;
		u--;
		for(; k > 0; k--)
		{
			cin >> v;
			v--;
			g[i].push_back(v);  // u->vへの有向辺
		}
	}

	queue<int> que;
	vector<bool> seen(n, false);
	vector<int> dist(n, -1);  // 頂点0からの距離
	que.push(0);
	seen[0] = true;
	dist[0] = 0;
	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : g[v])
		{
			if(seen[e]) continue;
			seen[e] = true;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}

	for(i = 0; i < n; i++)
	{
		cout << i+1 << " " << dist[i] << endl;
	}

	return 0;
}
