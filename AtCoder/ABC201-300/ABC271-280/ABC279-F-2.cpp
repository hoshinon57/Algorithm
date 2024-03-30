#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC279 https://atcoder.jp/contests/abc279

/*
 * 最初にACした実装。
 * UnionFindに加え、setのマージテクを用いて解いた。
 * 
 * 考え方はABC279-F.cppと同じ。
 * あちらはbox[X]を「箱Xに入っているボールの代表ボール」としたが、
 * こちらはsetで全ボールを持たせた。
 * ボールが分離することは無いので、クエリ1についてはマージテクを用いることで十分に間に合う。
 * 
 * 代表ボールはsetの.begin()にて代用した。
 * 
 * 公式解説を踏まえて考えると、箱Xへのアクセスについて
 *   setの.begin()で先頭ボールにアクセス
 *   UnionFindeで代表ボールにアクセス
 * という違いで、考え方は同じだと思う。
 * ならばsetを使わない方がより良いかなと。
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

const int BMAX = 600000;  // ボールの最大数 (初期値N=3*10^5, クエリでQ回=3*10^5増加)
int main(void)
{
	// 1-indexed
	int i;
	int N, Q; cin >> N >> Q;
	vector<set<int>> box(N+1);  // box[i]:箱iに入っているボール集合
	vector<int> ba(BMAX+1);  // ba[j]:ボールjはどの箱に入っているか
	UnionFind uf(BMAX+1);
	for(i = 1; i <= N; i++)
	{
		box[i].insert(i);  // 箱iにはボールiが入っている
		ba[i] = i;  // ボールiは箱iに入っている
	}
	int tot = N;  // 現在のボール数

	while(Q > 0)
	{
		int op; cin >> op;
		if(op == 1)
		{
			int x, y; cin >> x >> y;
			int xb = -1, yb = -1;  // 箱Xの先頭のボール番号 -1ならその箱は空
			if(box[x].size() > 0) xb = *box[x].begin();
			if(box[y].size() > 0) yb = *box[y].begin();

			// ボールをマージ X <= Y
			if(box[x].size() < box[y].size())
			{
				swap(box[x], box[y]);
			}
			for(auto &e: box[y]) box[x].insert(e);
			box[y].clear();

			// UF統合
			if(xb != -1 && yb != -1)
			{
				uf.unite(uf.root(xb), uf.root(yb));
			}
			// ba[]更新
			// swapする前にybを取得しているので、swap実施の有無は処理に無関係となる
			if(yb != -1)
			{
				ba[uf.root(yb)] = x;
			}
		}
		else if(op == 2)
		{
			int x; cin >> x;
			tot++;
			if(box[x].size() > 0)  // 箱xにボールがあれば、UF統合
			{
				uf.unite(uf.root(*box[x].begin()), tot);
			}
			box[x].insert(tot);
			ba[tot] = x;
		}
		else
		{
			int x; cin >> x;
			cout << ba[uf.root(x)] << endl;
		}
		Q--;
	}

	return 0;
}
