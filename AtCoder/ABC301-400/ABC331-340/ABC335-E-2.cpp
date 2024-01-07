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
 * 公式解説を元に、UnionFindで実装してみた。
 * 同じAiである頂点をまとめるという考え方。
 *   https://atcoder.jp/contests/abc335/editorial/9037
 * 
 * グラフ構築にて、A[*]が異なるu,vは小⇒大の方向へ辺を張る。
 * A[*]が同一の場合は、辺を張らずにUnionFindで頂点を統合する。
 * 
 * priority_queueは {Ai, 頂点i} の形で持ち、Aiが小さい順に探索する。
 * 事前に全頂点についてキューに積んでおく。
 * キューから取り出した頂点vについて、v->e となる辺がある頂点eについて
 *   sc[e.root] = max(sc[e.root], sc[v.root]+1)
 * という遷移になる。
 * 
 * [注意点]
 * ・スコアscには常にuf.root()でアクセスする必要がある。
 * ・scの初期値に-1はNG. -INFとする。
 *   頂点1よりAiが小さい頂点から探索がスタートするため。
 * ・ある頂点について複数回探索しないようにするには、sc[]ではなくseenを用いる。
 * 
 * 実装を両方試してみて、ダイクストラ法っぽいやり方(ABC335-E.cpp)の方がやりやすかったかな。
 */

struct UnionFind
{
	vector<int> parent;  // parent[x]:xの親の番号 xが根であれば-1
	vector<int> siz;  // siz[x]:xの属するグループの頂点数

	// 頂点数nで初期化
	UnionFind(int n) : parent(n, -1), siz(n, 1) {}

	// 根を求める
	int root(int x)
	{
		if(parent[x] == -1) return x;  // xが根であれば、xを返す
		else
		{
			parent[x] = root(parent[x]);
			return parent[x];
		}
	}

	// xとyが同じグループに属するか
	bool issame(int x, int y)
	{
		return root(x) == root(y);  // 根が同一なら同じグループ
	}

	// xを含むグループとyを含むグループを併合する
	bool unite(int x, int y)
	{
		// x,yをそれぞれ根まで移動させる
		x = root(x);
		y = root(y);

		// すでに同じグループであれば何もしない
		if(x == y) return false;

		// 頂点数が少ない方が子になるようにする
		if(siz[x] < siz[y])
		{
			// xをyの子にする
			parent[x] = y;
			siz[y] += siz[x];
		}
		else
		{
			// yをxの子にする
			parent[y] = x;
			siz[x] += siz[y];
		}
		return true;
	}

	// xを含むグループの頂点数
	int size(int x)
	{
		return siz[root(x)];
	}
};

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	Graph g(N);
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		if(a[u] < a[v]) g[u].push_back(v);
		else if(a[u] > a[v]) g[v].push_back(u);
		else uf.unite(u, v);
	}

	vector<bool> seen(N, false);
	vector<int> sc(N, -INF32);  // 初期値-1はNG. 頂点1よりAiが小さい頂点から探索がスタートするため
	using vi = vector<int>;  // {Ai, 頂点i}
	priority_queue<vi, vector<vi>, greater<vi>> que;
	for(i = 0; i < N; i++)  // Ai順にアクセスするので、全頂点をキューに積んでおく
	{
		que.push({a[i], i});
	}
	sc[uf.root(0)] = 1;  // scにはuf.rootでアクセス
	while(!que.empty())
	{
		auto vc = que.top();
		que.pop();
		int v = vc[1];

		if(seen[v]) continue;
		seen[v] = true;

		int vr = uf.root(v);
		for(auto &e : g[v])
		{
			int er = uf.root(e);
			sc[er] = max(sc[er], sc[vr]+1);
//			que.push({a[e], e});  // 事前に全頂点をキューに積んでいるため不要
		}
	}

	int ans = sc[uf.root(N-1)];
	if(ans < 0) ans = 0;
	cout << ans << endl;

	return 0;
}
