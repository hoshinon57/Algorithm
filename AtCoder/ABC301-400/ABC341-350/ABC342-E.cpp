#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC342 https://atcoder.jp/contests/abc342

/*
 * コンテスト中に解けず、解説を見た。
 * (通常とは少し違う)ダイクストラ法で解く。
 * 
 * f(S)の大きい順、つまり「駅Nに到着できる時刻が最も遅い順（余裕がある順）」にf(S)を確定させていく。
 * これは
 *   入力A,BについてB->Aの逆方向に辺を張る
 *   f(N)=INFとし、f(S)の大きい順に確定させていく
 * といった通常とは逆のダイクストラで解くことができる。
 * 
 * f(B)からf(A)を確定させるとき、Aからは遅くとも f(B)-c に出発する必要がある。
 * これを l,l+d,...,l+d*(k-1) の間から探す。
 * 最初二分探索が必要かと思ったが、O(1)で計算できた。実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・どの順にf(S)を確定させていくか？
 *   ⇒f(S)の大きい順になりそう。
 *   通常のダイクストラは「始点からの距離が小さい順」に確定させていくので、
 *   処理を変えることで「距離が大きい順」でのダイクストラで実装できるのではないか。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll l, d, k, c;
};
using Graph = vector<vector<Edge>>;

// 頂点sを起点にダイクストラ
// 手持ちのライブラリを「大きい方から確定させていく」ように手直ししている
void dijkstra(Graph &G, int s, vector<ll> &dist)
{
	dist[s] = INF64;
	using dist_v = pair<ll, int>;  // 距離とその頂点番号のpair
	priority_queue<dist_v> que;  // 大きい方が優先度が高い
	que.push({dist[s], s});

	while(!que.empty())
	{
		ll d = que.top().first;
		int v = que.top().second;
		que.pop();

		if(d < dist[v]) continue;  // {d, v}はゴミのためスルー

		// 頂点vから出ている各辺について探索
		for(auto &e : G[v])  // 辺e
		{
			/*
			 * 本来は e.to->v へと張られている辺。
			 * dist[e.to] + e.c <= dist[v] が条件。よって dist[v]-e.c < 0 はNG.
			 * e.toの出発時刻は、 dist[v]-e.c をリミットとしてなるべく遅い電車となる。
			 * これを l,l+d,...,l+d*(k-1) の間から探す。
			 */
			ll tmp = dist[v] - e.c;
			if(tmp < 0) continue;  // 乗れる電車が無い
			tmp = (tmp - e.l) / e.d;  // 0-indexedで、何本目の電車か
			chmin(tmp, e.k-1);  // 最終電車で上限クリップ
			// tmpには e.to->v への電車のうち、何番目の電車に乗るべきかという値が入っている

			ll d_next = e.l + e.d * tmp;
			if(dist[e.to] < d_next)  // 大きい方へ更新
			{
				dist[e.to] = d_next;
				que.push({dist[e.to], e.to});
			}
		}
	}
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		ll l, d, k, c; cin >> l >> d >> k >> c;
		int a, b; cin >> a >> b;  // a->b
		a--; b--;
		// 逆方向 b->a へ辺を張る
		g[b].push_back({a, l, d, k, c});
	}
	vector<ll> dist(N, -1);  // -1:頂点Nから到達不可
	dijkstra(g, N-1, dist);

	for(i = 0; i < N-1; i++)
	{
		if(dist[i] != -1) cout << dist[i] << endl;
		else cout << "Unreachable" << endl;
	}

	return 0;
}
