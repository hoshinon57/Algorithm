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

// ABC385 https://atcoder.jp/contests/abc385

/*
 * コンテスト時に解けず、解説を見た。
 * 
 * 基本は公式解説の通り。
 * 赤頂点vについて全探索し、
 * 隣接した頂点からxとして何個採用するかも全探索する。
 * 
 * xの採用手順は、vに隣接する頂点の次数が大きい順にするのが最適。
 * これだとO(N^2)になりそうだが、xとして採用するのは辺と同じだとみなすと、
 * 全体で2(N-1)回に過ぎないため全体の計算量としてはO(N)ですむ。
 * ※実際はソートが絡むのでO(NlogN).
 * 
 * v,xが決まったら、xのうち次数が最小のものがyとなるため、
 * N-(1+x+xy) が削除する頂点数。これの最小を求める。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・コンテスト時、全方位木DPにとらわれて結局解けなかった。
 *   とはいえこの方針は思いつかなかったと思うな…
 * ・赤頂点vについて全探索するのは思いつけると思うが、
 *   次の「xについて全探索、計算量は全体でO(N)に抑えられる」が厳しい。
 *   辺ごとに定数回しか操作しないのであれば、全体でO(N)に抑えられるという考え方を覚えておく。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	int ans = INF32;
	for(int v = 0; v < N; v++)  // 赤頂点vを全探索
	{
		vector<int> deg;  // (vに隣接する青頂点の次数)-1 -1はvを除外するため これがyとなる
		for(auto &e : g[v]) deg.push_back(g[e].size()-1);
		sort(deg.begin(), deg.end(), greater<int>());  // 降順
		for(int x = 1; x <= (int)deg.size(); x++)  // 正整数xを全探索
		{
			int y = deg[x-1];
			chmin(ans, N-(1+x+x*y));
		}
	}
	cout << ans << endl;

	return 0;
}
