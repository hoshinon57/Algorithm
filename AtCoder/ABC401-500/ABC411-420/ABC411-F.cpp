#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC411 https://atcoder.jp/contests/abc411

/*
 * WAが取れず最後まで不完全燃焼… 一応ACはしたが…
 * 時間がもったいないのでこれでおしまいとする。以下をほぼ流用している。
 *   https://x.com/Nichi10p/status/1937087354940105172
 *   https://atcoder.jp/contests/abc411/submissions/67023698
 * 
 * 頂点ごと「隣接する頂点」をsetで持って処理していく。
 * setのマージテクで使われる「sizeを見てswap」をするとWAだった。外すとAC.
 * WAなのはおそらく隣接頂点のuu,vvで不整合になるから。
 * UnionFindのuniteはサイズで調整しているので、swapせずともACしたのかな…
 * 
 * 「じゃあsetのマージにてsizeに応じて処理を変えたいときはどうする？」となったとき、分からず。
 * UnionFindを使わないでやりくりするんかなあ…
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
	// 0-indexed
	ll i;
	ll N, M; cin >> N >> M;
	UnionFind uf(N);
	vector<set<ll>> neli(N);  // neli[i]:頂点iの隣接頂点リスト ただしiはuf.rootでアクセスする
	vector<ll> u(M), v(M);
	for(i = 0; i < M; i++)
	{
		cin >> u[i] >> v[i];
		u[i]--; v[i]--;
		neli[u[i]].insert(v[i]);
		neli[v[i]].insert(u[i]);
	}

	ll ans = M;
	ll Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll x; cin >> x;
		x--;
		ll uu = uf.root(u[x]);
		ll vv = uf.root(v[x]);
		if(uf.issame(uu, vv))
		{
			;
		}
		else
		{
			// ufの根へマージしたい
			// u->v方向に処理したいので、root!=vならswapする
			uf.unite(uu, vv);
			if(uf.root(vv) != vv) swap(uu, vv);

			ans -= neli[uu].size();
			ans -= neli[vv].size();
			ans++;  // u-v間の辺が二重に減らされたので

			// u->vへマージ

			// 以下のswapがあるとダメだった。おそらく隣接頂点のuu,vvで不整合になるから。
			// UnionFindのuniteはサイズで調整しているので、swapせずともACした。
			// if(neli[uu].size() > neli[vv].size()) swap(neli[uu], neli[vv]);

			for(auto &e : neli[uu])
			{
				neli[e].erase(uu);
				neli[e].insert(vv);
			}
			for(auto &e : neli[uu])
			{
				neli[vv].insert(e);
			}
			neli[vv].erase(vv);
			ans += neli[vv].size();
		}
		cout << ans << endl;
	}

	return 0;
}
