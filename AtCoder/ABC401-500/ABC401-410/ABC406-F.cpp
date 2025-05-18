#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
#include <limits>  // numeric_limits
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC406 https://atcoder.jp/contests/abc406

/*
 * コンテスト時に解けず、解説を見た。
 * オイラーツアーとセグメント木を用いて解く。
 * 
 * 入力の木に対してオイラーツアーを適用すると、クエリ2の(根から遠い方の)部分木は
 * path[]における[vin[v],vout[v]]にて表現できる。
 * これと区間加算のセグメント木を用いると、解ける。
 * (木全体の総和をtotとし、根から遠い方の部分木の総和をsubとすると、(tot-sub)とsubのdiffが答)
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・根付き木に対するクエリは、オイラーツアーにより「区間に対するクエリ」に変換されるという考え方。
 * 
**/

using Graph = vector<vector<int>>;

// 木gについて、頂点vを基点にオイラーツアーを実施。
// path, vin, voutは関数内で初期化されるので未初期化でよい。(サイズは木の頂点数をNとして、pathは2N-1, vinとvoutはNになる)
// path:通った頂点の順番
// vin, vout:各頂点iについて、最初ないし最後に通ったタイミング
// 参考：
//   https://maspypy.com/euler-tour-%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7
//   https://scrapbox.io/ganariya-competitive/%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E3%83%84%E3%82%A2%E3%83%BC%E3%80%80%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA
//   https://obsidian.bibliophage.jp/Article/%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E3%83%84%E3%82%A2%E3%83%BC/
//   https://drken1215.hatenablog.com/entry/2018/08/14/193500
void dfs_tree_euler_tour(Graph &g, int s, vector<int> &path, vector<int> &vin, vector<int> &vout)
{
	// [memo]pathのサイズについて
	//   辺ごとに行き帰りで2回処理される。
	//   辺の数は(N-1)なので2(N-1)回処理。それに基点vが1回足されるので、2(N-1)+1=2N-1.

	int N = g.size();
	path.assign(2*N-1, 0);
	vin.assign(N, 0);
	vout.assign(N, 0);

	auto dfs = [&](auto &self, int &ord, int v, int p = -1) -> void
	{
		path[ord] = v;
		vin[v] = vout[v] = ord;  // voutを設定するのは、葉である頂点用
		ord++;
		for(auto &e : g[v])
		{
			if(e == p) continue;
			self(self, ord, e, v);
			path[ord] = v;
			vout[v] = ord;
			ord++;
		}
	};
	int o = 0;
	dfs(dfs, o, s);
}

// (1)Update(x, val) : 要素xをvalで更新する
// (2)Query(a, b) : 区間[a,b)にある要素のモノイド積を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Get(i) : 要素iを取得する
// (5)max_right(a, b, f) : [a,b)の範囲で、aを左端としてf(node)=trueとなる最右の要素番号を返す
// (6)min_left(a, b, f)  : [a,b)の範囲で、bを右端としてf(node)=trueとなる最左の要素番号を返す
// (7)debug(step, width, l, r) : デバッグ出力
// [注意]
//   0-indexed, および半開区間で処理する。
//   要素に配列を乗せる場合、vectorだとなぜか遅い。arrayか生配列(をstructでラップ)を使うこと。
// 以下URLをほぼそのまま持ってきている
// https://algo-logic.info/segment-tree/
// https://tsutaj.hatenablog.com/entry/2017/03/29/204841
template <typename T>
struct SegmentTree
{
private:
	using FX = function<T(T, T)>;  // X*X->X となる関数の型
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	FX fx;  // モノイドX上での二項演算
	const T ex;  // モノイドX上の単位元
	vector<T> node;

public:
	// 要素数で初期化
	SegmentTree(int size, FX fx_, T ex_) : fx(fx_), ex(ex_)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, ex);  // 単位元で初期化
	}

	// 要素xをvalで更新する
	void Update(int x, T val)
	{
		x += n-1;  // node[]の要素番号に変換
		// 最下段のnodeを更新したら、あとは親へ上って更新していく
		node[x] = val;
		while(x > 0)
		{
			x = (x-1)/2;  // 親
			node[x] = fx(node[2*x+1], node[2*x+2]);
		}
	}

	// 区間[a,b)にある要素のモノイド積を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	T Query(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値(=単位元)を返す
		if(r <= a || b <= l) return ex;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = Query(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = Query(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return fx(vl, vr);
	}

	// セグ木上の二分探索(max_right, min_left)の参考
	// 以下、セグ木上の要素をAi, クエリの値をvvとする
	// ・Ai>=vvとなる最左を求めたい、またはAi<vvとなる最右を求めたい
	//   Range Maxにて f(T x) { return x<vv; } としてmax_rightを使う (Ai<vvの間true)
	//   Range Minにて f(T x) { return x>=vv; } としてmin_leftを使う (Ai>=vvの間true)

	// [a,b)の範囲で、aを左端としてf(node)=trueとなる最右(ret)を返す。つまり[a,ret)がtrueで、[a,ret+1)がfalse.
	// b=Nとして呼び出すことが多いはず。
	// 全区間trueになる場合はret=bを返す。[a,b)=trueなので。
	// 全区間falseになる場合はret=aを返す。[a,a).
	// 
	// 左端からf(node)=falseを探していくイメージ。
	// 
	// f:セグ木の要素(型T)を引数に取り、bool型を返す関数を渡す
	// k:内部処理用。node[k]としてアクセス
	// l,r:内部処理用。nodeの[l,r)を対象とする。再帰にて変化する
	int max_right(int a, int b, function<bool(T)> f, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 今見ている区間[l,r)でf()=trueである、または
		// クエリ[a,b)と対象[l,r)が交わらない
		if(f(node[k]) || (r <= a || b <= l)) return b;  // 自身の右隣を返す

		if(k >= n-1) return l;  // 自分が葉なら、その位置を返す
		// 葉なので、lが位置を表している

		int vl = max_right(a, b, f, 2*k+1, l, (l+r)/2);  // 左側からf(node)=falseを探していく
		if(vl != b)  // 左側に答(falseとなる点)がある
		{
			return vl;
		}
		else
		{
			return max_right(a, b, f, 2*k+2, (l+r)/2, r);
		}
	}

	// [a,b)の範囲で、bを右端としてf(node)=trueとなる最左(ret)を返す。つまり[ret,b)がtrueで、[ret-1,b)がfalse.
	// a=0として呼び出すことが多いはず。
	// 全区間trueになる場合はret=aを返す。[a,b)=trueなので。
	// 全区間falseになる場合はret=bを返す。[b,b).
	// 
	// 右端からf(node)=falseを探していくイメージ。
	// 
	// f:セグ木の要素(型T)を引数に取り、bool型を返す関数を渡す
	// k:内部処理用。node[k]としてアクセス
	// l,r:内部処理用。nodeの[l,r)を対象とする。再帰にて変化する
	int min_left(int a, int b, function<bool(T)> f, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 今見ている区間[l,r)でf()=trueである、または
		// クエリ[a,b)と対象[l,r)が交わらない
		if(f(node[k]) || (r <= a || b <= l)) return a;  // 自身の左端を返す

		if(k >= n-1) return l+1;  // 自分が葉なら、その位置を返す
		// 葉なので、lが位置を表している
		// かつ、lがf(node[k])=falseとなる左端なので、l+1が目的の値となる

		int vl = min_left(a, b, f, 2*k+2, (l+r)/2, r);  // 右側からf(node)=falseを探していく
		if(vl != a)  // 右側に答(falseとなる点)がある
		{
			return vl;			
		}
		else
		{
			return min_left(a, b, f, 2*k+1, l, (l+r)/2);
		}
	}

	// 要素xをvalで更新する
	// Update()と違い、木全体の更新は行わない。Build()の呼び出しが必要。
	// 用途：初期化時に全要素を設定し、Build()で木を構築する
	void Set(int i, T val)
	{
		i += n-1;  // node[]の要素番号に変換
		node[i] = val;
	}

	// 初期化時にSet()で全要素を設定した後、Build()にて木を構築する
	void Build(void)
	{
		// 最下段を除いた右下の要素から、左上方向へ構築していく
		int i;
		for(i = n-2; i >= 0; i--)
		{
			node[i] = fx(node[2*i+1], node[2*i+2]);
		}
	}

	// 要素iを取得する
	T Get(int i)
	{
		i += n-1;  // node[]の要素番号に変換
		return node[i];
	}

#if 0
	// デバッグ出力 [i,i+width)でiはstepごと増加、全体は[l,r)でQueryを呼び出していく
	// debug(1, (好みの値)) とかで呼び出すのが多そうか
	// 要素の型によってはcoutできないものもあるため、ビルドエラー防止で#if 0としている
	void debug(int step, int width, int l = -1, int r = -1)
	{
		if(l == -1) { l = 0; r = n; }  // 引数無しは全区間
		cout << "debug:[" << l << "," << r << ")" << endl;
		for(int i = l; i < r; i += step)
		{
			cout << " [" << i << "," << i+width << "):" << this->Query(i, i+width) << endl;
		}
	}
#endif
};

int main(void)
{
	// 頂点a,bのどちらが根寄りかはvinで判断できる。vinが小さい方が根寄り。

	int i;
	int N; cin >> N;
	Graph g(N);
	vector<pair<int,int>> ed;  // i番目の辺の2頂点
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
		ed.push_back({u, v});
	}
	vector<int> path, vin, vout;
	dfs_tree_euler_tour(g, 0, path, vin, vout);

	// Range Sum Query(RSQ)
	using T = ll;
	auto fx = [](T x1, T x2) -> T { return x1+x2; };
	T ex = 0;
	SegmentTree<T> seg(N*2, fx, ex);
	for(i = 0; i < N; i++)
	{
		seg.Update(vin[i], 1);
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			ll x, w; cin >> x >> w;
			x--;
			seg.Update(vin[x], seg.Get(vin[x])+w);
		}
		else
		{
			ll y; cin >> y;
			y--;
			auto [u,v] = ed[y];
			// uが根寄りになるようにする
			if(vin[u] > vin[v]) swap(u, v);
			ll tot = seg.Query(0, 2*N);
			ll sub = seg.Query(vin[v], vout[v]+1);
			ll ans = abs((tot-sub) - sub);
			cout << ans << endl;
		}
	}

	return 0;
}
