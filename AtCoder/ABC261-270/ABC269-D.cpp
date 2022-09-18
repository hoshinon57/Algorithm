#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC269 https://atcoder.jp/contests/abc269

/*
 * Union-Findで解く。
 * グループ数はN個とし(0～N-1)、入力のマスの番号をグループ番号とする。
 * 
 * 以下の情報を用意し、入力時に設定する。
 * black[x][y] : (x,y)が黒なら、black[x][y]=1
 * grid_idx[x][y] : // (x,y)が黒のとき、それが何番目のマスか (0～N-1)
 * このとき、配列の添字が負にならないよう、+1005のゲタをはかせる。
 * 
 * 各マスごとに、隣接する周囲6マスが黒であるか、それぞれblack[][]にて調べていく。
 * 黒ならそのマスとは連結しているため、Union-Findにて統合処理を行う。
 * 
 * Union-Findにおいて root(i)=i となる個数が連結成分の個数となるので、
 * 最後にこれを出力すればよい。
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

// x,yが -1000～+1000 の範囲で、入力時に+1005のゲタをはかせるので、
// 配列のサイズは 1000+1005 より少し多い2010とした。

vector<pair<int,int>> xy;  // xy[i]:入力におけるi番目のマスのx,y(0-indexed)
int black[2010][2010] = {0};  // (x,y)が黒なら、black[x][y]=1
int grid_idx[2010][2010] = {0};  // (x,y)が黒のとき、それが何番目のマスかをgrid_idx[x][y]に保持 (0～N-1)

int main(void)
{
	// 0-indexed
	int i, j;
	int N;
	cin >> N;
	for(i = 0; i < N; i++)
	{
		int x, y;
		cin >> x >> y;
		x += 1005;  // 配列の添字が負にならないよう、+1005のゲタをはかせる
		y += 1005;
		xy.push_back({x,y});
		black[x][y] = 1;
		grid_idx[x][y] = i;
	}
	// ここまで入力

	int dx[6] = {-1, -1,  0, 0, 1, 1};
	int dy[6] = {-1,  0, -1, 1, 0, 1};
	UnionFind uf(N);  // グループN個 番号は入力におけるマスの番号を使う(0～N-1)
	for(auto &e : xy)
	{
		int x = e.first;
		int y = e.second;
		for(j = 0; j < 6; j++)  // 隣接のマス数は6種類
		{
			// 隣のマスが黒なら統合する
			if(black[x+dx[j]][y+dy[j]] == 1)
			{
				// 何番目のマスかを、grid_idxを使って取得
				int idx1 = grid_idx[x][y];
				int idx2 = grid_idx[x+dx[j]][y+dy[j]];

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
