#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC408 https://atcoder.jp/contests/abc408

/*
 * ORの最小値なので、上位bitから決めていくのが良さそう、となる。
 *   最上位bitを0か1かだったら、もちろん0にしたい。
 *   まず、最上位bitが0の辺だけを使ってみて、頂点1->Nへ行けるかをチェック。
 *   行けるなら、最上位bitが1の辺を捨てる。ansの最上位bitは0.
 *   行けないなら、辺は残す。ansの最上位bitは1.
 *   残っている辺を用いて、次のbitも同じようにチェック。
 * なイメージ。
 * 
 * 公式解説(snukeさん)と見比べて、細かい実装の違いは以下2つ。
 * 1)到達判定なので、BFSではなくUnionFindで良い。UFの方がずっと早い。
 *   (まあDFS/BFSも想定解のはずなので、時間制限3sになってると思うが)
 * 2)辺を残す/捨てるではなく、ansで上位から確定済みのbitを使って判定していた。
 *   これは賢いな… とはいえ自分がやったら実装に手間取りそう…
**/

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

using Graph = vector<vector<int>>;
// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}
	return;
}

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> u(M), v(M), w(M); for(i = 0; i < M; i++) {cin >> u[i] >> v[i] >> w[i]; u[i]--; v[i]--;}
	vector<bool> use(M, true);  // その辺がまだ生きてるならtrue
	ll ans = 0;
	for(ll b = 32; b >= 0; b--)  // b bit目を決める
	{
		Graph g(N);
		for(i = 0; i < M; i++)
		{
			if(!use[i]) continue;
			if(isbiton(w[i], b)) continue;  // b bit目が0のやつだけ使う
			g[u[i]].push_back(v[i]);
			g[v[i]].push_back(u[i]);
		}
		vector<int> dist(N, INF32);
		bfs_graph(g, 0, dist);
		if(dist[N-1] != INF32)  // 到達OK
		{
			// ansのb bit目は0にできる
			// b bit目が1の辺を消す
			for(i = 0; i < M; i++)
			{
				if(isbiton(w[i], b)) use[i] = false;
			}
		}
		else  // 到達NG
		{
			// ansのb bit目は1にする必要がある
			// 辺は全て残す
			setbit(ans, b);
		}
	}
	cout << ans << endl;

	return 0;
}
