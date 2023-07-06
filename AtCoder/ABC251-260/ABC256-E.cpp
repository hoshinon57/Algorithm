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

// ABC256 https://atcoder.jp/contests/abc256

/*
 * 深さ優先探索にて解く。
 * 
 * 人i⇒人Xiへの有向辺を張っていったときに、この順にキャンディを配れば不満度はたまらない。
 * ただしFunctional Graphであることから、連結成分ごとに必ずサイクルは1つ存在するため、不満度を0にはできない。
 * よって、サイクル内で最も不満度の小さい部分を採用すればよい。
 * 
 * 未探索の頂点からDFSし、サイクルを検出する。
 * 以下の実装では、サイクル検出時に始点からサイクル終端までの頂点をhistoryに保持する。
 * ここからサイクル部分をたどり、不満度の最小部分を採用する。
 * 
 * これを未探索の頂点について実施し、総和が答となる。
 */

using Graph = vector<vector<int>>;
vector<bool> seen, finished;
vector<int> history;

// 頂点vを起点にDFS.
// サイクルを見つけたらtrueを返す。その場合、vからサイクル終端までをhistoryに保持する。
bool dfs(Graph &g, int v)
{
	seen[v] = true;
	history.push_back(v);  // vを行きがけ時に登録し、帰りがけ時に削除

	for(auto &e : g[v])
	{
		if(finished[e]) continue;
		if(seen[e] && !finished[e])
		{
			// サイクル検出した
			finished[v] = true;  // 頂点vについて探索完了
			history.push_back(e);  // サイクルの終端
			return true;
		}
		if(dfs(g, e))  // サイクル検出してreturnしてきた場合は、再帰的にreturnしていく
		{
			finished[v] = true;  // この場合も、頂点vについて探索完了扱いとする
			return true;
		}
	}

	finished[v] = true;
	history.pop_back();
	return false;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N; i++)
	{
		int x; cin >> x;
		x--;  // 0-indexed
		g[i].push_back(x);
	}
	vector<ll> c(N); for(i = 0; i < N; i++) {cin >> c[i];}
	seen.resize(N, false);
	finished.resize(N, false);

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 探索済み
		history.clear();

		if(!dfs(g, i)) continue;  // falseで戻ってきた場合、サイクルは見つからなかった

		// 逆順にした履歴を先頭から見て行って、先頭と同じ頂点が見つかるまでがサイクル
		reverse(history.begin(), history.end());
		int s = history[0];
		ll c_min = c[history[0]];  // サイクル内の不満度の最小値
		for(j = 1; j < (int)history.size(); j++)
		{
			if(history[j] == s) break;  // サイクル終了
			c_min = min(c_min,c[history[j]]);
		}	
		ans += c_min;
	}
	cout << ans << endl;

	return 0;
}
