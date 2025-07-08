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

// ABC413 https://atcoder.jp/contests/abc413

/*
 * コンテスト時はノータッチだったが、後に解説ACした内容。
 * UnionFindを用いて解く。
 * 
 * このユーザー解説が近いかな、と思う。
 *   https://atcoder.jp/contests/abc413/editorial/13414
 * 
 * マス数が多いので、障害物に着目して解きたい気持ちになる。
 * スタートとゴールが決まってるので、一例として
 *   グリッド枠外の上辺から、障害物をたどって下辺に行けるなら、No回答
 * となる。
 * 障害物をNo.0～(K-1), グリッド枠外の上辺、左辺、右辺、下辺をそれぞれK, K+1, K+2, K+3とする。
 * 8近傍で隣接する障害物をUnionFindで統合していき、最後に
 *   KとK+1 (上と左)
 *   KとK+3 (上と下)
 *   K+2とK+1 (右と左)
 *   K+2とK+3 (右と下)
 * のいずれかが統合されていれば、No回答となる。
**/

// 「問題解決力を鍛える!アルゴリズムとデータ構造」のcode11.3を写経したもの
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

	// 連結成分ごとに頂点をvector<int>で返す
	// ex) 頂点0,1,3と2,5が同一グループの場合、 {{0,1,3},{2,5},{4}} といった形で返す
	// 連結成分単位での順番は不定になると思われる
	// 計算量は頂点数をNとしてO(N)
	vector<vector<int>> groups(void) {
		vector<vector<int>> member(parent.size());  // member[v]:要素vを親とするグループ
		for(int v = 0; v < (int)parent.size(); v++) {
			member[root(v)].push_back(v);
		}
		// memberの空要素を削除して返す
		vector<vector<int>> res;
		for(auto &e : member) {
			if(!e.empty()) res.push_back(e);
		}
		return res;
	}
};

using pll = pair<ll,ll>;

int main(void)
{
	ll i, p, q;
	ll H, W, K; cin >> H >> W >> K;
	UnionFind uf(K+4);
	map<pll,ll> mp;  // {y,x} = idx
	vector<ll> y(K), x(K);
	for(i = 0; i < K; i++)
	{
		ll r, c; cin >> r >> c;
		r--; c--;
		mp[{r, c}] = i;
		y[i] = r;
		x[i] = c;
	}

	for(i = 0; i < K; i++)
	{
		ll yy = y[i];
		ll xx = x[i];
		for(p = -1; p <= 1; p++)  // 8近傍
		{
			for(q = -1; q <= 1; q++)
			{
				if(p == 0 && q == 0) continue;
				ll ny = yy+p;
				ll nx = xx+q;
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
				if(mp.count({ny, nx}) == 0) continue;  // そこに障害物が無い
				ll ni = mp[{ny, nx}];
				uf.unite(i, ni);
			}
		}
		// グリッドの端にある障害物は、グリッド枠外の仮想的な障害物と統合する
		if(yy == 0) uf.unite(i, K);
		if(yy == H-1) uf.unite(i, K+3);
		if(xx == 0) uf.unite(i, K+1);
		if(xx == W-1) uf.unite(i, K+2);
	}

	bool ok = true;
	if(uf.issame(K, K+1) || uf.issame(K, K+3) || uf.issame(K+2, K+1) || uf.issame(K+2, K+3)) ok = false;
	YesNo(ok);

	return 0;
}
