#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題25
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1160&lang=ja

/*
 * Union-Findで解く。
 * 
 * 陸のマス{i,j}に対して、その番号を0から連番で記録しておく。
 * 実装ではisland_id[]としている。
 * 
 * 周囲8方向の探索にて陸が見つかったとき、その2つの陸をUnion-Findで統合する。
 * 
 * 探索後、島の番号が根の番号と一致している数が、連結成分数となる。
 */

int w, h;
vector<vector<int>> area;  // area[i][j]:(i,j)が0なら海、1なら陸
vector<vector<int>> island_id;  // island_id[i][j]:UnionFindで使う (i,j)が陸のとき、その番号を0-indexedで保持する

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
	int i, j;
	while(true)
	{
		cin >> w >> h;
		if(w == 0 && h == 0) break;

		area.assign(h, vector<int>(w));
		island_id.assign(h, vector<int>(w));
		int n = 0;  // 陸のマス数
		for(i = 0; i < h; i++)
		{
			for(j = 0; j < w; j++)
			{
				cin >> area[i][j];
				if(area[i][j] == 1)  // UnionFind用に、陸のマスには0から順に番号を振る
				{
					island_id[i][j] = n;
					n++;
				}
			}
		}

		UnionFind uf(n);  // 陸のマス数で初期化 0-indexed
		for(i = 0; i < h; i++)
		{
			for(j = 0; j < w; j++)
			{
				if(area[i][j] != 1) continue;  // 陸ではない
				// {i,j}を起点に周囲8マスを探索
				// ※右下方向にi,jを進めるため左や上方向は探索不要だが、こちらの方がバグを埋め込みにくい
				for(int dy = -1; dy <= 1; dy++)
				{
					for(int dx = -1; dx <= 1; dx++)
					{
						int y = i + dy;
						int x = j + dx;
						if(y < 0 || y >= h || x < 0 || x >= w) continue;  // 範囲外ならスルー
						if(area[y][x] != 1) continue;  // 陸ではないならスルー
						// 2つの陸{i,j},{y,x}を統合する
						uf.unite(island_id[i][j], island_id[y][x]);
					}
				}
			}
		}
		int answer = 0;
		// 島の番号が根の番号と一致している数が、連結成分数となる
		for(i = 0; i < n; i++)
		{
			if(uf.root(i) == i) answer++;
		}
		cout << answer << endl;
	}

	return 0;
}
