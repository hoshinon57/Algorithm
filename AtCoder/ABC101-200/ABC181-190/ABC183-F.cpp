#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC183 https://atcoder.jp/contests/abc183

/*
 * マージテクで解く。
 * 
 * vectorとmapを用いて、mp[i][z]を「生徒iの集団にて、クラスzの生徒数」と定義する。
 * またUnion-Findにて集団そのものを管理する。
 * 
 * クエリ1では、a,bをUnion-Findにて統合しつつ、mpは小さい方を大きい方へマージする。
 * 実装では簡単にするために、b->aへマージする前提として、必要ならば事前にswapさせた。
 * これでクエリあたりの計算量をO(logN)に抑えることができる。(mapを使ってるのでO((logN)^2)かも)
 * 
 * クエリ2ではaの根をrとし、mp[r][b]を出力すれば良い。
 * 
 * [ACまでの思考の流れ]
 * ・集団そのものはUnion-Findを使えば良さそう。
 * ・クエリ2にて、クラスyの人数を求められるのが厄介。
 * ・順当に mp[a][b] を「生徒aの集団にてクラスbの人数」とする形か。
 *   集団は分離はしないから、クエリ1にて「小さい方から大きい方へ」マージすれば計算量を抑えられそう。
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
	// 1-indexed
	int i;
	int N, Q; cin >> N >> Q;
	UnionFind uf(N+1);
	vector<map<int,int>> mp(N+1);  // mp[i][z]:生徒iの集団にて、クラスzの生徒数
	for(i = 1; i <= N; i++)
	{
		int c; cin >> c;
		mp[i][c] = 1;
	}

	while(Q > 0)
	{
		Q--;
		int k, a, b; cin >> k >> a >> b;
		if(k == 1)
		{
			if(uf.issame(a, b)) continue;
			// 方針：b->aへマージする
			int a_r = uf.root(a);
			int b_r = uf.root(b);
			uf.unite(a, b);

			// 元のa,bの根のうち、a_rに現在の根が設定されるようにする
			if(a_r != uf.root(a)) swap(a_r, b_r);

			// b->aへマージしたいので、サイズはa_rの方を大きくする
			// [memo]使っているUFではマージテクを使った併合がされているので、この処理は無くても良いみたい
			if(mp[a_r].size() < mp[b_r].size()) swap(mp[a_r], mp[b_r]);

			// マージ
			for(auto& [key, val] : mp[b_r]) mp[a_r][key] += val;
		}
		else
		{
			int r = uf.root(a);
			int ans = 0;
			if(mp[r].count(b) > 0) ans = mp[r][b];  // 要素が無い場合はアクセスさせない
			cout << ans << endl;
		}
	}
	

	return 0;
}
