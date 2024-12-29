#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC075 https://atcoder.jp/contests/abc075

// 解説に「DFSでもできる」と書かれていたので、DFSで解いてみた版。
// 消す辺について全探索。
// 頂点0からDFSして未探索の頂点が残っていたら、その辺は橋。

using Graph = vector<vector<int>>;

void dfs(Graph &g, vector<int> &chk, int v)
{
	if(chk[v] == 0) return;  // 探索済み
	chk[v] = 0;
	for(auto &e : g[v])
	{
		dfs(g, chk, e);
	}
}

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<int> a(M), b(M); for(i = 0; i < M; i++) {cin >> a[i] >> b[i]; a[i]--; b[i]--;}
	int ans = 0;
	for(i = 0; i < M; i++)  // 消す辺i
	{
		Graph g(N);
		for(j = 0; j < M; j++)
		{
			if(j == i) continue;
			g[a[j]].push_back(b[j]);
			g[b[j]].push_back(a[j]);
		}

		// 頂点0からDFSして、chk[*]に1が残っていたら、辺iは橋
		vector<int> chk(N, 1);
		dfs(g, chk, 0);
		if(*max_element(chk.begin(), chk.end()) == 1) ans++;
	}
	cout << ans << endl;

	return 0;
}
