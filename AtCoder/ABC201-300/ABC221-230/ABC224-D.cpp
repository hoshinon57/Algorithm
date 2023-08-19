#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC224 https://atcoder.jp/contests/abc224

/*
 * 幅優先探索で解く。
 * ※公式解説とほぼ同じ実装だった。
 *   https://atcoder.jp/contests/abc224/editorial/2813
 * 
 * 以下、0-indexedで記載する。
 * 9文字の文字列strを、頂点pにコマiが置かれているとき str[p]=i と表現する。
 * 空きマスは"_"とする。
 * するとパズル完成は str="_01234567" となる。
 * 
 * これを初期状態から上記の完成形へ遷移可能か、BFSで解けばよい。
 * 「状態を文字列で持つ、dist[]をmapで管理する」など手直しはあるものの、通常のBFSと同じ考え方。
 */

using Graph = vector<vector<int>>;
// パズルの状態sを起点にBFS.
// 探索結果として、distにsを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, string s, map<string, int> &dist)
{
	queue<string> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		string v = que.front();
		que.pop();
		
		int empty_idx = v.find("_");  // 空きマスを探し、そこから繋がる頂点を探索
		for(auto &e : G[empty_idx])
		{
			// "_"と繋がる頂点を交換
			string v_new = v;
			swap(v_new[empty_idx], v_new[e]);
			if(dist.count(v_new) > 0) continue;  // 探索済み

			dist[v_new] = dist[v] + 1;
			que.push(v_new);
		}
	}
	return;
}

int main(void)
{
	// 0-indexed
	int i;
	Graph g(9);  // 9頂点
	int M; cin >> M;
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	string pzl = "_________";  // "_"が9個
	for(i = 0; i < 8; i++)
	{
		int p; cin >> p;
		p--;
		pzl[p] = (char)(i + '0');  // コマiは頂点pに置かれている
	}
	map<string, int> dist;  // 例：dist["_01234567"] = 10; "_"は空きマス
	bfs_graph(g, pzl, dist);

	int ans = -1;
	if(dist.count("01234567_") > 0) ans = dist["01234567_"];
	cout << ans << endl;

	return 0;
}
