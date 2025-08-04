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

// ABC417 https://atcoder.jp/contests/abc417

/*
 * 「辞書順最小は前から貪欲」の典型。
 * DFSで解く。解説はkyopro_friends氏のが良い。
 *   https://atcoder.jp/contests/abc417/editorial/13589
 * 
 * まず、DFSでX->Yへのパスを1つ見つける関数を考える。
 * ここから、構築したグラフにおける各g[v]の要素を昇順ソートしておくだけで、だいたい上手くいく。
 * 頂点番号が小さい頂点からたどっていけるので。
 * 
 * 考察ミスってDFS()から出るときにseenをリセットしてしまいTLE.
 * この処理は不要。ある頂点について過去に行ったことがあるならば、
 * 後から行こうとしてもそれは辞書順最小にならないため。
 * ※以下のような考え方。
 * -----------
 * (1)3-1-2-*****- と以前たどっていて、
 * (2)3-1-5-6-2-*** をたどるのはムダ。
 * (2)でゴールまで行けるなら、(1)の方がベターだし、
 * (1)でゴールまで行けなかったのなら、(2)でもゴールに行けない。
 * -----------
**/

using Graph = vector<vector<int>>;

void solve(void)
{
	int i;
	int N, M, x, y; cin >> N >> M >> x >> y;
	x--; y--;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	for(i = 0; i < N; i++)
	{
		sort(g[i].begin(), g[i].end());  // 辞書順最小のため、ソートしておく
	}

	vector<bool> seen(N, false);
	vector<int> ans;

	// 次にidx番目の頂点
	auto dfs = [&](auto &self, int v, int idx) -> bool
	{
		if(seen[v]) return false;
		seen[v] = true;
		ans.push_back(v);
		if(v == y)
		{
			int sz_ = (int)ans.size();
			for(i = 0; i < sz_; i++) {
				cout << ans[i]+1;
				if(i != sz_-1) cout << " ";
			}
			cout << endl;
			return true;
		}

		for(auto &e : g[v])
		{
			if(self(self, e, idx+1)) return true;
		}

		// seen[v] = false;  // これ不要 あるとTLEする
		ans.pop_back();
		return false;
	};
	dfs(dfs, x, 0);
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}

	return 0;
}
