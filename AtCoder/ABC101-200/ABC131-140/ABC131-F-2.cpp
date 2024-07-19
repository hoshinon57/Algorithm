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

// ABC131 https://atcoder.jp/contests/abc131

/*
 * けんちょん氏の解説を見て実装した内容。
 *   https://drken1215.hatenablog.com/entry/2019/06/23/004700
 * 
 * UnionFindにて、[0,MAX)をX座標、[MAX,MAX+MAX)をオフセット付きY座標として管理する。
 * そして入力の点(x,y)を、上記の2点間を結ぶ辺として管理する。
 * 
 * 各連結成分にて、操作の最終形は
 *   各X側の点と各Y側の点が全て結ばれている
 * 形となる。
 * つまり (X側の要素数)*(Y側の要素数).
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「グリッド上のマス」や「平面上の座標」をX,Yで分けて二部グラフとして考えてみる。
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
	const int MAX = 1e5+10;
	int i;
	int N; cin >> N;
	UnionFind uf(MAX*2);  // [0,MAX)がX座標、[MAX,MAX+MAX)がオフセット付きY座標
	for(i = 0; i < N; i++)
	{
		int x, y; cin >> x >> y;
		uf.unite(x, y+MAX);
	}
	vector<ll> cx(MAX*2), cy(MAX*2);
	for(i = 0; i < MAX; i++) cx[uf.root(i)]++;  // x
	for(i = MAX; i < MAX*2; i++) cy[uf.root(i)]++;  // y
	// ある入力の点(x1,y1)において、uf.root(x1)=uf.root(y1+MAX)となる
	ll ans = 0;
	for(i = 0; i < MAX*2; i++)
	{
		ans += cx[i] * cy[i];
	}
	cout << ans-N << endl;

	return 0;
}
