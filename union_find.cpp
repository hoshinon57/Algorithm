#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Union-Findのテンプレート
// 「問題解決力を鍛える!アルゴリズムとデータ構造」のcode11.3を写経したもの

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
 * [関連する問題]
 * ABC075-C
 * ABC120-D
 * ABC049-D
 * ABC126-E
 * ABC157-D
 * ABC177-D
 * ABC226-E
 * ABC229-E
 * ABC259-D
 * ABC264-E
 * ABC269-D
 * ABC292-D
 * ABC304-E
 * AOJ 1160 https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1160&lang=jp
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

int main(void)
{
	UnionFind uf(7);  // 0～6がそれぞれ異なるグループ
	
	uf.unite(1,2);
	uf.unite(2,3);  // {0}, {1,2,3}, {4}, {5}, {6}
	uf.unite(5,6);  // {0}, {1,2,3}, {4}, {5,6}
	cout << uf.issame(1,3) << endl;  // true
	cout << uf.issame(2,5) << endl;  // false

	uf.unite(1,5);  // {0}, {1,2,3,5,6}, {4}
	cout << uf.issame(2,5) << endl;  // 今度はtrue

	return 0;
}
