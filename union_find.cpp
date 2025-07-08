#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

// Union-Findのライブラリ

/*
 * ＞辺の追加と連結性の判定を得意とするデータ構造は Union Find である. 
 * https://kazun-kyopro.hatenablog.com/entry/ABC/264/E
 * 
 * グラフの連結成分数を求める場合、大事なのは「頂点からUnion-Findの番号を取得する手段」と考える。
 * 一般的なグラフであれば頂点番号＝Union-Findの番号、とみなせる。
 *   参考：book_drken/chapter11 のcode11-4.cpp
 * グリッドグラフであれば、マス{y,x}に対してuf_id[y][x]にUnion-Findの番号を保持しておく手段が考えられる。
 *   参考：AtCoder/past_questions_100 のpast-questions-25-4(uf).cpp
 * 
 * vectorで定義したい場合、以下のようにする。
 *   vector<UnionFind> uf(10, UnionFind(N));  // N頂点のを10個
 * 
 * [関連する問題]
 * ABC040-D
 * ABC075-C
 * ABC120-D
 * ABC049-D
 * ABC126-E
 * ABC131-F
 * ABC157-D
 * ABC177-D
 * ABC206-D
 * ABC214-D
 * ABC226-E
 * ABC229-E
 * ABC259-D
 * ABC264-E
 * ABC269-D
 * ABC279-F
 * ABC292-D
 * ABC304-E
 * ABC413-G
 * AOJ 1160 https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1160&lang=jp
 */

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

// [verify]ABC328-F,ABC373-D,ABC280-F,ABC087-D,AOJ:DSL_1_B
// 重み付きUnionFind
// けんちょん氏の実装をほぼそのままお借りしている
// https://drken1215.hatenablog.com/entry/2023/11/12/040700
// 参考：https://qiita.com/drken/items/cce6fc5c579051e64fab
// T:重みの型 int or ll  基本はllが安全と思われる
// 主に使うメソッド: root(), issame(), unite(), size(), diff()  diff以外は通常のUnionFindと同じ
template<class T> struct UnionFind_Weighted
{
	vector<int> par;
	vector<T> weight;

	// UnionFind_Weighted() { }

	// 頂点数Nで初期化
	UnionFind_Weighted(int N, T zero = 0) : par(N, -1), weight(N, zero) {}
	void init(int N, T zero = 0) {
		par.assign(N, -1);
		weight.assign(N, zero);
	}
	
	// 根を求める
	int root(int x) {
		if (par[x] < 0) return x;
		else {
			int r = root(par[x]);
			weight[x] += weight[par[x]];
			return par[x] = r;
		}
	}

	// xとyが同じグループに属するか
	bool issame(int x, int y) {
		return root(x) == root(y);
	}

	// xを含むグループの頂点数
	int size(int x) {
		return -par[root(x)];
	}
	
	// v[y] - v[x] = w となるように、
	// xを含むグループとyを含むグループを併合する
	// x,yが併合済みの場合はfalseを返す (重み付きUnionFindとしての未定義動作)
	bool unite(int x, int y, T w) {
		w += get_weight(x), w -= get_weight(y);
		x = root(x), y = root(y);
		if (x == y) return false;
		if (par[x] > par[y]) swap(x, y), w = -w; // merge technique
		par[x] += par[y];
		par[y] = x;
		weight[y] = w;
		return true;
	}
	
	// get v[x]
	T get_weight(int x) {
		root(x);
		return weight[x];
	}
	
	// xとyが同じグループにいるとき、weight(y) - weight(x) を返す
	T diff(int x, int y) {
		return get_weight(y) - get_weight(x);
	}
	
	// get groups
	vector<vector<int>> groups() {
		vector<vector<int>> member(par.size());
		for (int v = 0; v < (int)par.size(); ++v) {
			member[root(v)].push_back(v);
		}
		vector<vector<int>> res;
		for (int v = 0; v < (int)par.size(); ++v) {
			if (!member[v].empty()) res.push_back(member[v]);
		}
		return res;
	}
	
	// debug
	friend ostream& operator << (ostream &s, UnionFind_Weighted uf) {
		const vector<vector<int>> &gs = uf.groups();
		for (const vector<int> &g : gs) {
			s << "group: ";
			for (int v : g) s << v << "(" << uf.get_weight(v) << ") ";
			s << endl;
		}
		return s;
	}
};

int main(void)
{
	UnionFind uf(7);  // 0～6がそれぞれ異なるグループ
	
	uf.unite(1,2);
	uf.unite(2,3);  // {0}, {1,2,3}, {4}, {5}, {6}
	uf.unite(5,6);  // {0}, {1,2,3}, {4}, {5,6}
	assert( uf.issame(1,3));
	assert(!uf.issame(2,5));
	{  // groups
		auto grp = uf.groups();
		vector<vector<int>> ans = {{0},{1,2,3},{4},{5,6}};
		// groups()の連結成分単位での順番は不定のはずなので、grp,ansともにソートしておく
		sort(grp.begin(), grp.end());
		for(auto &e : grp) sort(e.begin(), e.end());
		sort(ans.begin(), ans.end());
		for(auto &e : grp) sort(e.begin(), e.end());
		assert(grp == ans);
	}
	{
		UnionFind uf2(4);
		vector<vector<int>> ans = {{0},{1},{2},{3}};
		auto grp = uf2.groups();
		assert(grp == ans);
	}

	uf.unite(1,5);  // {0}, {1,2,3,5,6}, {4}
	assert(uf.issame(1,3));  // 今度はtrue

	UnionFind_Weighted<int> ufw(10);  // 重みはint型で定義
	ufw.unite(0, 1, 2);
	ufw.unite(9, 1, 5);
	assert( ufw.issame(0, 9));
    assert(!ufw.issame(2, 9));
	assert(ufw.diff(0, 9) == -3);
    assert(ufw.size(0) == 3);
    assert(ufw.size(9) == 3);
    assert(ufw.size(2) == 1);

	return 0;
}
