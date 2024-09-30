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

// ABC373 https://atcoder.jp/contests/abc373

// 重み付きUnionFindで解いた版

// 重み付きUnionFind
// けんちょん氏の実装をほぼそのままお借りしている
// https://drken1215.hatenablog.com/entry/2023/11/12/040700
// 参考：https://qiita.com/drken/items/cce6fc5c579051e64fab
// T:重みの型 int or ll  基本はllが安全と思われる
// 主に使うメソッド: root(), issame(), unite(), size(), diff()  diff以外は通常のUnionFindと同じ
template<class T> struct UnionFind_Weighted
{
	vector<int> par;
	vector<T> weight;

	// UnionFind_Weighted() { }

	// 頂点数Nで初期化
	UnionFind_Weighted(int N, T zero = 0) : par(N, -1), weight(N, zero) {}
	void init(int N, T zero = 0) {
		par.assign(N, -1);
		weight.assign(N, zero);
	}
	
	// 根を求める
	int root(int x) {
		if (par[x] < 0) return x;
		else {
			int r = root(par[x]);
			weight[x] += weight[par[x]];
			return par[x] = r;
		}
	}

	// xとyが同じグループに属するか
	bool issame(int x, int y) {
		return root(x) == root(y);
	}

	// xを含むグループの頂点数
	int size(int x) {
		return -par[root(x)];
	}
	
	// v[y] - v[x] = w となるように、
	// xを含むグループとyを含むグループを併合する
	// x,yが併合済みの場合はfalseを返す (重み付きUnionFindとしての未定義動作)
	bool unite(int x, int y, T w) {
		w += get_weight(x), w -= get_weight(y);
		x = root(x), y = root(y);
		if (x == y) return false;
		if (par[x] > par[y]) swap(x, y), w = -w; // merge technique
		par[x] += par[y];
		par[y] = x;
		weight[y] = w;
		return true;
	}
	
	// get v[x]
	T get_weight(int x) {
		root(x);
		return weight[x];
	}
	
	// xとyが同じグループにいるとき、weight(y) - weight(x) を返す
	T diff(int x, int y) {
		return get_weight(y) - get_weight(x);
	}
	
	// get groups
	vector<vector<int>> groups() {
		vector<vector<int>> member(par.size());
		for (int v = 0; v < (int)par.size(); ++v) {
			member[root(v)].push_back(v);
		}
		vector<vector<int>> res;
		for (int v = 0; v < (int)par.size(); ++v) {
			if (!member[v].empty()) res.push_back(member[v]);
		}
		return res;
	}
	
	// debug
	friend ostream& operator << (ostream &s, UnionFind_Weighted uf) {
		const vector<vector<int>> &gs = uf.groups();
		for (const vector<int> &g : gs) {
			s << "group: ";
			for (int v : g) s << v << "(" << uf.get_weight(v) << ") ";
			s << endl;
		}
		return s;
	}
};

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	UnionFind_Weighted<ll> ufw(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		ll w; cin >> w;  // u->vへ重みw
		ufw.unite(v, u, w);
	}

	for(i = 0; i < N; i++)
	{
		cout << ufw.diff(i, ufw.root(i));
		if(i != N-1) cout << " ";
	}
	cout << endl;

	return 0;
}
