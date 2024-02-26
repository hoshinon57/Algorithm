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

// 競プロ典型90問:26 https://atcoder.jp/contests/typical90/tasks/typical90_z

/*
 * 木は二部グラフなので、2色に塗り分けて多い色の方から(N/2)頂点を出力すればよい。
 * 
 * [ACまでの思考の流れ]
 * ・木は二部グラフ[典型]
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
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	vector<int> col(N, -1);
	dfs_bipartite(g, 0, col);

	// 過半数を持つ色が0,1どちらかを調べ、その頂点をansへ
	int cnt = 0;
	for(auto &e : col) if(e == 0) cnt++;
	int tar = (cnt >= N/2) ? 0 : 1;  // 過半数を持つ色
	vector<int> ans;
	for(i = 0; i < N; i++)
	{
		if(col[i] == tar) ans.push_back(i+1);
	}

	// 先頭N/2個を出力
	for(i = 0; i < N/2; i++)
	{
		cout << ans[i];
		if(i != N/2-1) cout << " ";
	}
	cout << endl;

	return 0;
}
