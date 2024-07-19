#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC131 https://atcoder.jp/contests/abc131

/*
 * x座標ごとに考えてみる。例えば、
 *   x=x1に存在する点のy座標={1,2,3,4}
 *   x=x2に存在する点のy座標={4,5,6}
 * のとき、(x,y)=({x1,x2}, {1,2,3,4,5,6}) の点は全て追加されるっぽい。
 * さらに(x3,6)といった点があると、ここもy座標が1～6の点は全て追加される。
 * 
 * よって、同じx座標にて登場するy座標をグルーピングすると上手くいきそう、と分かる。
 * UnionFindで行けそう、となった。
 * 
 * ここまでの考察を元に、以下方針で実装してAC.
 * ・入力の点をx座標ごとに管理。
 * ・次に構築のフェーズ。
 *   UnionFindにて、x座標が同じ点の、yの値を統合していく。
 * ・そして計算のフェーズ。
 *   各点のy座標がUnionFindにていくつの集合にまとめられているかを調べ、その数だけ答に加算。
 *   同じx座標ごとに重複カウントしないよう、mapで管理。
 * ・N個の点は最初から打たれているので、答から引いて出力。
 * 
 * [ACまでの思考の流れ]
 * ・操作の最終形がどうなっているかを考える。
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
	int i;
	int N; cin >> N;
	const int MX = 1e5;
	UnionFind uf(MX+5);
	vector<vector<int>> p(MX+5);  // p[x]={y1,y2,y3,...}:x座標がxであるような点の、y座標の一覧
	for(i = 0; i < N; i++)
	{
		int xx, yy; cin >> xx >> yy;
		p[xx].push_back(yy);
	}

	// UnionFind構築
	// x座標が同じ点の、yの値を統合
	for(auto &x : p)
	{
		for(i = 1; i < (int)x.size(); i++) uf.unite(x[0], x[i]);
	}

	ll ans = 0;
	for(auto &x : p)
	{
		map<int,int> chk;  // chk[y]:
		for(i = 0; i < (int)x.size(); i++)  // y=x[i]  全体でループN回
		{
			int y = x[i];
			int ry = uf.root(y);
			if(chk.count(ry) == 0)  // 各x座標ごとに重複カウントを避ける
			{
				ans += uf.size(ry);  // y座標の集合ぶん加算
				chk[ry] = 1;
			}
		}
	}
	ans -= N;
	cout << ans << endl;

	return 0;
}
