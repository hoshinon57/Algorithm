#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC392 https://atcoder.jp/contests/abc392

/*
 * 辺の入力を見ながら、非連結ならその辺を使い、すでに連結であればそれは「余り」として管理する。
 * この余りは ama[*]={{idx,u},...} として辺の番号と片側の頂点番号で管理する。
 * 
 * 余り{idx,u}について先頭から見ていく。
 * 頂点0から順にuと非連結なら接続。
 * ただし毎度頂点0からチェックしていくとTLEするので、次は先ほど繋いだ頂点の次からチェックしていく。
 * これで解ける。
 * 
 * 非連結である2頂点をつなぐと、連結成分数は1つ減る。
 * グラフ全体が連結になるまであとx回つなぐ必要があるとすると、どの2頂点でもよいので非連結なxペアを接続すれば、全体が連結になる。
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

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	UnionFind uf(N);
	vector<pair<int,int>> ama;  // ama[*] = {{idx,u}, {idx,u}, ...} 余ってる辺(u,v)のidxとuを登録していく
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		if(uf.issame(a, b)) ama.push_back({i, a});
		else uf.unite(a, b);
	}

	int ans = 0;
	vector<array<int,3>> dt;
	int tar = 0;
	for(auto [idx,u] : ama)
	{
		for(i = tar; i < N; i++)  // 余りの辺を、先頭の頂点から見ていって、非連結ならつなぐ
		{
			if(uf.issame(u, i)) continue;

			uf.unite(u, i);
			dt.push_back({idx+1, u+1, i+1});
			ans++;
			break;
		}
		tar = i;  // 次回はここから
		if(i == N) break;  // 連結した
	}

	cout << ans << endl;
	for(auto [e1,e2,e3] : dt) cout << e1 << " " << e2 << " " << e3 << endl;

	return 0;
}
