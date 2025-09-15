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

// ABC420 https://atcoder.jp/contests/abc420

/*
 * クエリ2用に、各頂点の色を保持しておく。実装ではcol[].
 * またUnionFindを用いて、連結成分ごとに黒頂点の個数を管理する。実装ではcnt[].
 * 
 * コンテスト時、クエリ1にてu,vが同じ連結成分にあるケースを見逃しており1WA.
 * ＞各クエリについて、そのクエリより前にu,vを結ぶ辺は追加されていない。
 * よりu,vは非連結と勘違いしてしまっていた。
**/

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
	ll N, Q; cin >> N >> Q;
	UnionFind uf(N);
	vector<ll> cnt(N);  // cnt[i.root]: 頂点iが属する連結成分における、黒頂点の数
	vector<ll> col(N);  // 1なら黒
	while(Q > 0)
	{
		Q--;
		int ki; cin >> ki;
		if(ki == 1)
		{
			ll u, v; cin >> u >> v;
			u--; v--;
			if(uf.issame(u, v)) continue;
			ll cu = cnt[uf.root(u)];
			ll cv = cnt[uf.root(v)];
			uf.unite(u, v);
			cnt[uf.root(u)] = cu+cv;
		}
		if(ki == 2)
		{
			ll v; cin >> v;
			v--;
			if(col[v] == 0)  // 白⇒黒
			{
				cnt[uf.root(v)]++;
			}
			else  // 黒⇒白
			{
				cnt[uf.root(v)]--;
			}
			col[v] = 1-col[v];
		}
		if(ki == 3)
		{
			ll v; cin >> v;
			v--;
			YesNo(cnt[uf.root(v)] > 0);
		}
	}

	return 0;
}
