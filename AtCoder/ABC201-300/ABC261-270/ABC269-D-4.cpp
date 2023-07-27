#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC269 https://atcoder.jp/contests/abc269

/*
 * ABC269-D.cppとほぼ同じ。Union-Findで解く。
 * 
 * 変化点としては、black[]およびgrid_idx[]を、それぞれsetとmapに変更した。
 * 意図は以下。
 * ・blackに統合できるため、配列xy[]が不要になる
 * ・配列の添字が無くなるため、「ゲタをはかせる」という処理が不要になる、もしくは
 *   「ゲタをはかせる」という処理を思いつけなくても解けるようにする練習
 * ・情報を二次元配列ではなくset/mapを使って表現する練習
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
	// 0-indexed
	int i, j;
	int N;
	cin >> N;
	set<pair<int,int>> black;  // 黒で塗られているマス(x,y)の集合
	map<pair<int,int>, int> grid_idx;  // (x,y)が黒のとき、それが何番目のマスかをgrid_idx[{x,y}]に保持 (0～N-1)

	for(i = 0; i < N; i++)
	{
		int x, y;
		cin >> x >> y;
		black.insert({x,y});
		grid_idx[{x,y}] = i;
	}
	// ここまで入力

	int dx[6] = {-1, -1,  0, 0, 1, 1};
	int dy[6] = {-1,  0, -1, 1, 0, 1};
	UnionFind uf(N);  // グループN個 番号は入力におけるマスの番号を使う(0～N-1)
	for(auto &e : black)
	{
		int x = e.first;
		int y = e.second;
		for(j = 0; j < 6; j++)  // 隣接のマス数は6種類
		{
			// 隣のマスが黒なら統合する
			if(black.find({x+dx[j], y+dy[j]}) != black.end() )
			{
				// 何番目のマスかを、grid_idxを使って取得
				int idx1 = grid_idx[{x,y}];
				int idx2 = grid_idx[{x+dx[j], y+dy[j]}];

				uf.unite(idx1, idx2);
			}
		}
	}

	// Union-Findにおける連結成分の個数は、root(i)==iとなる個数で求められる
	int answer = 0;
	for(i = 0; i < N; i++)
	{
		if(uf.root(i) == i) answer++;
	}
	cout << answer << endl;

	return 0;
}
