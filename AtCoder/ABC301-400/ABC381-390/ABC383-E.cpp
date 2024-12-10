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

// ABC383 https://atcoder.jp/contests/abc383

/*
 * コンテスト時に解けず、解説を見た。
 * 貪欲法およびクラスカル法っぽく解く。以下、基本的に公式解説の通り。
 * 
 * 連結成分ごと、AおよびBでペアを未作成の頂点を管理する。 -> na,nb
 * 辺を重みの小さい順に見ていって、辺をつなぐごとに
 * ・片方の連結成分に含まれるnaと、もう片方に含まれるnbをマッチングさせる。
 *   nbとnaという組み合わせも同様。
 *   連結成分を統合したらna,nbも更新する。
 * としていけばよい。
 * 
 * 連結成分ごと、Aの残り数は情報として必要だが、Aの具体的な頂点番号までは管理不要、というのがポイントか。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・UnionFindで統合していくのかなーとは思い浮かんだが、
 *   未マッチングの頂点番号をどう管理したらよいのかが分からなかった。
 *   「パスに含まれる辺の重みの最大値」であることから、頂点数だけ分かればよくて具体的な頂点番号までは不要であった。
 * ・具体的な頂点番号を保持するのは難しくても、頂点数を保持しておけば十分、という考え方を覚えておく。
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

	// 連結成分ごとに頂点をvector<int>で返す
	// ex) 頂点0,1,3と2,5が同一グループの場合、 {{0,1,3},{2,5},{4}} といった形で返す
	// 連結成分単位での順番は不定になると思われる
	// 計算量は頂点数をNとしてO(N)
	vector<vector<int>> groups(void) {
		vector<vector<int>> member(parent.size());  // member[v]:要素vを親とするグループ
		for(int v = 0; v < (int)parent.size(); v++) {
			member[root(v)].push_back(v);
		}
		// memberの空要素を削除して返す
		vector<vector<int>> res;
		for(auto &e : member) {
			if(!e.empty()) res.push_back(e);
		}
		return res;
	}
};

// MSTに使う用の辺
// 辺番号など情報を追加したい場合は、メンバ変数およびコンストラクタ、"<"演算子に手を入れること
struct Edge
{
	int u, v;  // 頂点
	int w;  // 辺の重み
	Edge(void) {}
	Edge(int u_, int v_, int w_) : u(u_), v(v_), w(w_) {}
	bool operator<( const Edge &right ) const
	{
		return this->w < right.w;
	}
};

int main(void)
{
	ll i;
	ll N, M, K; cin >> N >> M >> K;
	vector<Edge> edge;
	for(i = 0; i < M; i++)
	{
		int u, v, w; cin >> u >> v >> w;
		u--; v--;
		edge.push_back({u, v, w});
	}
	sort(edge.begin(), edge.end());
	vector<int> na(N), nb(N);  // na[i]:親が頂点iの連結成分にて、未使用のAの残り
	for(i = 0; i < K; i++)
	{
		int a; cin >> a; a--;
		na[a]++;
	}
	for(i = 0; i < K; i++)
	{
		int b; cin >> b; b--;
		nb[b]++;
	}

	UnionFind uf(N);
	ll ans = 0;
	for(auto [u, v, w] : edge)
	{
		if(uf.issame(u, v)) continue;
		// グループ1:u グループ2:v
		ll g1_a = na[uf.root(u)], g2_b = nb[uf.root(v)];
		ll g1_b = nb[uf.root(u)], g2_a = na[uf.root(v)];

		// グループ1側のnaとグループ2側のnbをマッチング
		ll mi = min(g1_a, g2_b);
		ans += mi * w;
		g1_a -= mi; g2_b -= mi;

		// グループ1側のnbとグループ2側のnaをマッチング
		mi = min(g1_b, g2_a);
		ans += mi * w;
		g1_b -= mi; g2_a -= mi;

		uf.unite(u, v);
		na[uf.root(u)] = g1_a + g2_a;
		nb[uf.root(u)] = g1_b + g2_b;
	}
	cout << ans << endl;

	return 0;
}
