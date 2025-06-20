#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC401 https://atcoder.jp/contests/abc401

/*
 * おおよそ公式解説と近い。
 * 「消すべき頂点」をlog付けてsetで実装したのが差異か。
 * (set無し実装だと200ms, setありだと600msぐらいかなーという感じ)
 * 
 * 「頂点や辺をグラフから削除するのは大変、逆から見て増やす方向で考える」という典型がある。
 * 本問ではkが増えるごとに辺を追加していくことから、kが小さい方から見ていくのが良さそう、となる。
 * 
 * 小問題kのとき、頂点kと辺で結ばれている頂点xについて、
 * x>kならばxは削除必要。
 * 逆にx<kなら辺で結んでよい。
 * 前者をsetで管理し、後者はUnionFindでつなげる。
 * 
 * 各kごと、頂点1のUFの連結成分数がk個なら、条件を満たせる。このとき前述のsetの要素数が答。
 * そうでなければ-1を出力。
**/

using Graph = vector<vector<int>>;

// 「問題解決力を鍛える!アルゴリズムとデータ構造」のcode11.3を写経したもの
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

int main(void)
{
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	set<int> s;  // 削除必要な頂点
	UnionFind uf(N);
	for(i = 0; i < N; i++)
	{
		s.erase(i);  // まずは削除必要な頂点から除外
		for(auto &e : g[i])
		{
			if(e > i) s.insert(e);
			else uf.unite(e, i);  // e<=i 自分より番号が小さい頂点と結ぶ
		}
		if(uf.size(0) == (i+1)) cout << s.size() << endl;
		else cout << -1 << endl;
	}

	return 0;
}
