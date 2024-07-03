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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC335 https://atcoder.jp/contests/abc335

/*
 * コンテスト中に解けず、解説を見た。
 * ダイクストラ法に近いやり方で解く。hamamu氏のツイートを参考にした。
 *   https://twitter.com/hamamu_kyopro/status/1743630492526936366
 * 
 * Aiに同じ値が存在しない場合、広義ではなく狭義単調増加となる。
 * この場合は頂点1からDPで解ける。
 * ※DAGになるので自作のトポロジカルソートライブラリを使うとか、
 *   priority_queueを使ってAiの小さい順に確定させていくとか、
 *   そんな感じになるだろうか。
 * 
 * Aiに同じ値が存在するのが厄介。上記(※)を少し改良して
 * 「Aiの小さい順、Aiが等しければスコアの大きい順に確定させていく」
 * とすると上手くいきそう。
 * 
 * するとグラフは入力u,vについて、
 *   a[*]の小さい方から大きい方へ辺を張る。
 *   同一なら双方向に張る。
 * としておき、キュー操作は
 *   今見ている頂点vからeへの遷移について、
 *   a[v]<a[e]ならスコアを+1してキューに積む。
 *   a[v]=a[e]ならスコアをそのままキューにに積む。
 *   (頂点vがスコア確定済みならスルー)
 * とすればよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・トポロジカルソートっぽいのは途中で気づいたが、自作のライブラリでは上手く解けなかった。
 *   priority_queueを使って探索していくという手法も覚えておく。
 * ・グラフ探索はある頂点に対して2回以上は探索させたくない。
 *   そのためにはどういう順序/条件で探索順を決めていくか。
 *   今回はAiと、スコアが「大きい」順が適切だった。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		if(a[u] < a[v]) g[u].push_back(v);
		else if(a[u] > a[v]) g[v].push_back(u);
		else  // 双方向
		{
			g[u].push_back(v);
			g[v].push_back(u);
		}
	}
	
	vector<int> sc(N, -1);
	// Aiの小さい順、Aiが等しければスコアの大きい順に探索する
	// スコアを-1して保持することで、「小さい順」のpriority_queueを使える
	using vi = vector<int>;  // {Ai, 頂点iのスコア の-1倍, 頂点i}  小さい方が優先度が高い
	priority_queue<vi, vector<vi>, greater<vi>> que;
	que.push({a[0], -(1), 0});  // 頂点0スタート
	while(!que.empty())
	{
		auto vc = que.top();
		que.pop();
		int s = -(vc[1]);  // -1してキューに積んでいるため
		int v = vc[2];  // 頂点v
		if(sc[v] != -1) continue;  // 探索済み
		sc[v] = s;
		for(auto &e : g[v])
		{
			if(a[v] < a[e])
			{
				que.push({a[e], -(s+1), e});
			}
			else  // a[v] == a[e]  スコアの変化は無し
			{
				que.push({a[e], -s, e});
			}
		}
	}

	int ans = 0;
	if(sc[N-1] != -1) ans = sc[N-1];
	cout << ans << endl;

	return 0;
}
