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

// ABC280 https://atcoder.jp/contests/abc280

/*
 * 重み付きUnionFindで解いてみた版。
 * 考え方はABC280-F.cppと同じ。まずはそちらを参照。
 * 
 * inf[i]: 頂点iの連結成分がinfにできるならtrue
 * として定義する。
 * 
 * M本の辺追加にて、以下の手順で操作する。
 * (1)a,bが既に連結の場合
 *   追加しようとしている辺と、現在のa,bの差に矛盾がなければ、何もしない。
 *   矛盾があれば、infをtrueにする。
 * (2)a,bが非連結の場合
 *   a,bを連結するが、その前にa,bのinfがtrueであれば、連結後もinf=trueとなる。
 * 
 * クエリは以下の手順で操作する。
 * (1)x,yが連結の場合
 *   inf=trueならinfを出力。
 *   inf=falseならdiffを出力。
 * (2)x,yが非連結の場合
 *   nanを出力。
 */

// [verify]ABC328-F,ABC373-D
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
	ll i;
	ll N, M, Q; cin >> N >> M >> Q;
	UnionFind_Weighted<ll> ufw(N);
	vector<bool> inf(N, false);  // その連結成分がinfにできるならtrue
	for(i = 0; i < M; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		a--; b--;
		if(ufw.issame(a, b))
		{
			if(ufw.diff(b, a) == c) {;}  // 矛盾なし 何もしない
			else inf[ufw.root(a)] = true;  // 矛盾あり
		}
		else  // 非連結
		{
			bool flg = false;
			if(inf[ufw.root(a)] || inf[ufw.root(b)]) flg = true;  // a,bどちらかがinfなら、連結してもinfにできる
			ufw.unite(b, a, c);
			inf[ufw.root(a)] = flg;
		}
	}
	while(Q > 0)
	{
		Q--;
		ll x, y; cin >> x >> y;
		x--; y--;
		if(ufw.issame(x, y))
		{
			if(inf[ufw.root(x)]) cout << "inf" << endl;
			else cout << ufw.diff(y, x) << endl;
		}
		else cout << "nan" << endl;
	}

	return 0;
}
