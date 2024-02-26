#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:12 https://atcoder.jp/contests/typical90/tasks/typical90_l

/*
 * UnionFindで解く。
 * 公式解説とほぼ同じ実装だった。
 *   https://twitter.com/e869120/status/1381739128291614720
 *   https://github.com/E869120/kyopro_educational_90/blob/main/sol/012.cpp
 * 
 * 自分の実装では上下左右に1マスずつ番兵的なマスを用意しておき、処理を簡易にした。
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
	int H, W; cin >> H >> W;
	H += 2; W += 2;  // 上下左右に1マスずつ番兵
	vector<vector<int>> col(H, vector<int>(W, 0));  // col[H][W] 0:白 1:赤
	UnionFind uf(H*W);
	// マス(y,x)からUnionFind用のIDを取得
	auto toID = [&](int y, int x) -> int
	{
		return y*W + x;
	};

	int Q; cin >> Q;
	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	int i;
	while(Q > 0)
	{
		Q--;
		int t; cin >> t;
		if(t == 1)
		{
			int r, c; cin >> r >> c;
			col[r][c] = 1;
			for(i = 0; i < DIR; i++)
			{
				int id1 = toID(r, c);
				if(col[r+dy[i]][c+dx[i]] == 1)
				{
					int id2 = toID(r+dy[i], c+dx[i]);
					uf.unite(id1, id2);
				}
			}
		}
		else
		{
			int ra, ca, rb, cb; cin >> ra >> ca >> rb >> cb;
			bool res = true;
			if(col[ra][ca] == 0 || col[rb][cb] == 0)
			{
				res = false;
			}
			else
			{
				int id1 = toID(ra, ca);
				int id2 = toID(rb, cb);
				res = uf.issame(id1, id2);
			}
			YesNo(res);
		}
	}

	return 0;
}
