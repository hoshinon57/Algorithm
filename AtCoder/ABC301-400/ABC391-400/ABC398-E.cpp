#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC398 https://atcoder.jp/contests/abc398

/*
 * 木は二部グラフ。[典型]
 * 
 * 頂点を2色で塗り分けたとき、奇閉路ができないようにするには頂点が偶数個、
 * つまり異なる色の2頂点を結ぶことになる。（かつ、直接繋がっていない2頂点）
 * また異なる色の2頂点を結んだグラフも二部グラフのまま。
 * 
 * 選べる頂点のペアを最初にリストアップしていき、使えるものを順に使っていけばよい。
 * 最初にリストアップした個数が奇数か偶数で先手後手が決まる。
 * 
 * ※コンテスト時、選べる頂点ペアをリストアップするのにワーシャルフロイド法を用いた。
 *   これでも問題なかったが、毎ターンワーシャルフロイドを実施してたためTLE.
 *   全Nターン、ワーシャルフロイド1回ごとO(N^3)で全体でO(N^4). N=100より間に合うかと思ったが…
 *   インタラクティブ問題だし、入出力が通常と違うので時間がかかったのかなあ。
**/

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
bool dfs_bipartite(Graph &g, int v, vector<int> &col, int cur = 0)
{
	col[v] = cur;
	for(auto &e : g[v])
	{
		if(col[e] != -1)  // もう塗られている
		{
			if(col[e] == col[v]) return false;
		}
		else  // まだ塗っていない
		{
			int nxt = cur^1;
			if(!dfs_bipartite(g, e, col, nxt)) return false;
		}
	}
	return true;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	Graph g(N);
	vector<vector<bool>> ed(N, vector<bool>(N, false));  // ed[N][N]
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
		ed[a][b] = ed[b][a] = true;
	}
	vector<int> col(N, -1);
	dfs_bipartite(g, 0, col);

	set<pair<int,int>> s;  // 結べる頂点ペアの一覧 {u,v}でu<vとする
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			if(col[i] != col[j] && !ed[i][j])  // 異なる色、かつ隣接していない
			{
				s.insert({i, j});
			}
		}
	}

	int t = 0;
	if(s.size()%2 == 1)
	{
		cout << "First" << endl;
		t = 0;
	}
	else
	{
		cout << "Second" << endl;
		t = 1;
	}

	while(true)
	{
		if(t == 0)  // 自ターン
		{
			auto [u, v] = *s.begin();
			s.erase({u, v});
			cout << u+1 << " " << v+1 << endl;
		}
		else
		{
			int u, v; cin >> u >> v;
			u--; v--;
			if(u == -2 && v == -2) break;
			if(u>v) swap(u, v);
			s.erase({u, v});
		}
		t = 1-t;
	}

	return 0;
}
