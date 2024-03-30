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
 * UnionFindを用いて解く。
 * ※最初はUnionFindに加えsetのマージテクを用いて解いたが、
 * 　公式解説を元にマージテク無しでの実装をしたのがこのソースコード。
 *     https://atcoder.jp/contests/abc279/editorial/5284
 * 
 * 「ボールXがどの箱にあるか」を愚直に管理するとTLE.
 * UnionFindを用いて、ボールの集合の代表1つのみ、「どの箱にあるか」を管理すればよい。
 * ⇒なぜなら、ボールの集合は分離することは無いから。
 * 
 * box[i]:箱iに入っているボールの、代表番号 箱が空なら-1
 * ba[j]:ボールjはどの箱に入っているか
 * uf:UnionFindでボールの集合を管理
 * とする。
 * 
 * クエリ1：
 * 箱X,Yともにボールが入っている場合、box,ba,ufを更新する。
 * box[Y]を-1にするのを忘れずに。
 * 箱Yが空なら何もしない。
 * 箱Xが空なら、一部処理を簡略化する。
 * 
 * クエリ2：
 * UF統合し、box[X],ba[]を更新。
 * 箱Xが空ならUF統合は不要。
 * 
 * クエリ3：
 * ba[uf.root(x)] でOK.
 * 
 * [ACまでの思考の流れ]
 * ・「ボールXがどの箱にあるか」を愚直に管理するとTLEする。
 * ・ほしい情報は、
 * 　箱Xには何のボールがあるか？ もっと言えば箱Xの代表ボールは何か？
 * 　ボールXはどの箱にあるか？ もっと言えば「ボールXの集合の代表ボール」はどの箱にあるか？
 * ・ボールは分離することは無いこともあり、
 * 　ボールの集合はUnionFindで管理すれば良さそう。
 * 　これによりほしい情報も簡単に管理できる。
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
	vector<int> box(N+1);  // box[i]:箱iに入っているボールの、代表番号 箱が空なら-1
	vector<int> ba(BMAX+1);  // ba[j]:ボールjはどの箱に入っているか
	UnionFind uf(BMAX+1);
	for(i = 1; i <= N; i++)
	{
		box[i] = uf.root(i);  // =i と同じ  箱iにはボールiのみ入っており、代表番号はi
		ba[i] = i;  // ボールiは箱iに入っている
	}
	int tot = N;  // 現在のボール数

	while(Q > 0)
	{
		Q--;
		int op; cin >> op;
		if(op == 1)
		{
			int x, y; cin >> x >> y;
			if(box[y] == -1)  {;}  // 箱Yが空なら何もしない
			else if(box[x] == -1)  // Xが空
			{
				box[x] = uf.root(box[y]);  // 右辺はyである点に注意
				box[y] = -1;
				ba[uf.root(box[x])] = x;
			}
			else  // X,Yともにボールが入っている
			{
				// 更新したいもの：box, ba, uf
				uf.unite(uf.root(box[x]), uf.root(box[y]));
				box[x] = uf.root(box[x]);
				box[y] = -1;
				ba[uf.root(box[x])] = x;
			}
		}
		else if(op == 2)
		{
			int x; cin >> x;
			tot++;
			if(box[x] != -1)
			{
				uf.unite(uf.root(box[x]), tot);
				box[x] = uf.root(box[x]);
			}
			else
			{
				box[x] = tot;
			}
			ba[uf.root(tot)] = x;
		}
		else
		{
			int x; cin >> x;
			cout << ba[uf.root(x)] << endl;
		}
	}

	return 0;
}
