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

// ABC352 https://atcoder.jp/contests/abc352

/*
 * 最小全域木で解く。
 * 
 * 入力の頂点集合Sについて相互の辺を追加していき、MSTを求めればよい。
 * (MSTを求めた際にunionfindのsize()がNでなければ、全体は非連結のため-1を出力する)
 * 
 * ただし愚直に相互の辺を張ると、K頂点に対しO(K^2)の処理が必要となりTLEする。
 * 超頂点か…？と考えたが、結局のところ
 *   A1-A2-A3-...-Ak
 * と隣同士の頂点にのみ辺を張れば上手くいくっぽい。これならK頂点に対しO(K)で処理できる。
 * 未証明だが、そんな感じはしたので提出したらACした。
 * ※A1,A2,A3間にコストCの辺を張ることを考えると、
 *   A1-A2-A3と2辺を張った後、A1,A3間の辺についてはすでに連結かつコストも同一のため不要、
 *   というのがクラスカル法の気持ちから分かる…感じかな。
 * 
 * ※コンテスト時に全体の辺数の計算でバグっており苦戦した。
 *   そもそも実装力も低かった。反省。
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

// MSTに使う用の辺
// 辺番号など情報を追加したい場合は、メンバ変数およびコンストラクタ、"<"演算子に手を入れること
struct Edge
{
	int u, v;  // 頂点
	ll w;  // 辺の重み
	Edge(void) {}
	Edge(int u_, int v_, ll w_) : u(u_), v(v_), w(w_) {}
	bool operator<( const Edge &right ) const
	{
		return this->w < right.w;
	}
};

int main(void)
{
	// 0-indexed
	ll i, j;
	ll N, M; cin >> N >> M;
	vector<Edge> edge;
	for(i = 0; i < M; i++)
	{
		ll k, c; cin >> k >> c;
		vector<int> a(k);
		for(j = 0; j < k; j++)
		{
			cin >> a[j];
			a[j]--;
		}
		for(j = 1; j < k; j++)
		{
			edge.push_back({a[j-1], a[j], c});
		}
	}
	sort(edge.begin(), edge.end());

	UnionFind uf(N);
	ll ans = 0;
	for(auto &e : edge)
	{
		if(uf.issame(e.u, e.v)) continue;
		ans += e.w;
		uf.unite(e.u, e.v);		
	}
	if(uf.size(0) != N) ans = -1;  // 全体が非連結
	cout << ans << endl;

	return 0;
}
