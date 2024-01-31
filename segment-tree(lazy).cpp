#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>  // function
#include <limits>  // numeric_limits
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 抽象化版セグメント木のメモや実装
// ★注意★ #include <functional> を忘れずに。ローカル環境では無くてもビルドが通るが、AtCoderではCEになる。

/*
 * [ざっくり概要]
 * ・任意の区間の値を更新する(区間更新), 一律加算する(区間加算)
 * ・任意の区間における最小値や合計値を取得する
 * といった処理をO(logN)でできるデータ構造。
 * 要素には任意の作用付きモノイドを用いることができる(抽象化)。
 * 
 * LazySegmentTree:
 *   以下の操作をO(logN)で処理できる。
 *   (1)Update(a, b, x) : 区間[a,b)の要素をxを用いて更新する
 *   (2)Query(a, b) : 区間[a,b)にある要素にfxを作用させた値を返す
 *   0-indexed, および半開区間で処理する。
 *   コンストラクタには以下を指定する。
 *     	size:要素数, fx_,fa_,fm_:二項演算,
 *      fp_:区間和など区間に比例した作用素のときに使う二項演算。不要なときはnullptr.
 *      ex_,em_:単位元
 *      exは fx(x,ex_)=x となるものを指定するイメージ。
 *      emは「この値なら作用させない」「作用させる値として取りえない値」となるものを指定するイメージ。INF64でも良さそう。
 *      要素は単位元で初期化される。
 *   ★代表的なfx,ex等はmain()に記述している。
 * 
 * [Tips]
 * ・木の最下段のノード数は、問題文にて指定されるsize以上の2のべき乗。
 *   これをNとすると、最下段のノード数はN, それより上の段のノードは全部でN-1.
 *   よって木全体で必要なノード数は 2N-1 となる。
 * ・要素xをnode[]の添字番号に変換する場合：N-1を加算する
 * ・親から子へ行く場合、 k -> 2k+1, 2k+2
 * ・子から親へ行く場合、 k -> (k-1)/2  (切り捨て)
 * 
 * [参考資料]
 *   https://algo-logic.info/segment-tree/
 *   https://tsutaj.hatenablog.com/entry/2017/03/30/224339
 * 
 * [関連する問題 / verifyした問題]
 *   AOJ DSL_2_F(RMQ and RUQ) https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F&lang=ja
 *   AOJ DSL_2_G(RSQ and RAQ) https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G
 *   AOJ DSL_2_H(RMQ and RAQ) https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_H
 *   AOJ DSL_2_I(RSQ and RUQ) https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_I&lang=ja
 *   ABC035-C
 *   ABC185-F
 *   ABC327-F
 */

// (1)Update(a, b, x) : 区間[a,b)の要素をxを用いて更新する
// (2)Query(a, b) : 区間[a,b)にある要素にfxを作用させた値を返す
// [未実装](3)Find_Leftmost(a, b, x) : 区間[a,b)の範囲で、x以下となる最も左側の要素番号を返す
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/30/224339
// https://algo-logic.info/segment-tree/
template <typename X, typename M>
struct LazySegmentTree
{
private:
    using FX = function<X(X, X)>;
    using FA = function<X(X, M)>;
    using FM = function<M(M, M)>;
	using FP = function<M(M, ll)>;  // 区間和など、区間に比例した作用素用
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
    FX fx;  // モノイドX上での二項演算
    FA fa;
    FM fm;
	FP fp = [](M m_, ll n_) -> M { return m_; };  // デフォルトでは区間比例は無し
	const X ex;  // モノイドX上での単位元
	const M em;  // モノイドM上での単位元 (lazyがこの値なら何も作用させないイメージ)
	vector<X> node;  // 値配列
	vector<M> lazy;  // 遅延配列

	// k番目のnodeについて遅延評価を行う
	void Evaluate(int k, int l, int r)
	{
		// 以下3つの観点で実施する
		//   子ノードの遅延配列に値を伝播させる
		//   自ノードの値配列に値を伝播させる
		//   自分のノードの遅延配列を空にする
		if(lazy[k] == em) return;  // 更新するものが無い
		if(r-l > 1)  // 最下段でなければ、子へ伝搬させる
		{
			lazy[2*k+1] = fm(lazy[2*k+1], lazy[k]);
			lazy[2*k+2] = fm(lazy[2*k+2], lazy[k]);
		}
		// 自身を更新
//		node[k] = fa(node[k], lazy[k]);
		node[k] = fa(node[k], fp(lazy[k], r-l));
		lazy[k] = em;  // lazyを空にするイメージ
	}

public:
	// 要素数で初期化
	// fp_:区間和など、区間に比例した作用が無ければnullptr
	LazySegmentTree(int size, FX fx_, FA fa_, FM fm_, FP fp_, X ex_, M em_) : fx(fx_), fa(fa_), fm(fm_), ex(ex_), em(em_)
	{
		if(fp_ != nullptr) fp = fp_;  // 引数指定があれば設定
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, ex);  // 単位元で初期化
		lazy.resize(2*n-1, em);
	}

	// 区間[a,b)の要素をxを用いて更新する
	void Update(int a, int b, M x, int k = 0, int l = 0, int r = -1)
	{
		// 非遅延セグ木と違い、最上段から設定していくイメージ

		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// [memo]
		// クエリと対象が交わらない場合でも、評価は実施必要
		// 自身(=子)の親の計算にて、最後に子のnode[]を参照しているため、
		// lazy[]からnode[]へ値を反映させておく必要がある
		Evaluate(k, l, r);

		// クエリ[a,b)と対象[l,r)が交わらないので、何もしない
		if(r <= a || b <= l) return;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b)
		{
			lazy[k] = fm(lazy[k], x);
			Evaluate(k, l, r);
			return;
		}

		// 左右の子について再帰的に探索
		Update(a, b, x, 2*k+1, l, (l+r)/2);  // 左側
		Update(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		node[k] = fx(node[2*k+1], node[2*k+2]);
	}

	// 区間[a,b)にある要素にfxを作用させた値を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	X Query(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値(=単位元)を返す
		if(r <= a || b <= l) return ex;

		Evaluate(k, l, r);

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		X vl = Query(a, b, 2*k+1, l, (l+r)/2);  // 左側
		X vr = Query(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return fx(vl, vr);
	}

#if 0  // [ToDo]実装する
	// [a,b)の範囲で、x以下となる最も左側の要素番号を返す
	// 範囲内にx以下が見つからなければ、b(=範囲外)を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	int Find_Leftmost(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		Evaluate(k, l, r);

		// 自分の値がxより大きい   もしくは
		// クエリ[a,b)と対象[l,r)が交わらない
		if(node[k] > x || (r <= a || b <= l)) return b;  // 自身の右隣を返す

		if(k >= n-1) return l;  // 自分が葉なら、その位置を返す
		// 葉なので、lが位置を表している

		int vl = Find_Leftmost(a, b, x, 2*k+1, l, (l+r)/2);  // 左側
		if(vl != b)  // 左側に答がある
		{
			return vl;
		}
		else
		{
			return Find_Leftmost(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		}
	}
#endif

	// 要素iをvalで更新する
	// Update()と違い、木全体の更新は行わない。Build()の呼び出しが必要。
	// 用途：初期化時に全要素を設定し、Build()で木を構築する
	void Set(int i, X val)
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
	X Get(int i) { return Query(i, i+1); }
};

void Test(void)
{
	int i;
	int n = 6;
	using X = int;
	using M = int;
	auto fx = [](X x1, X x2) -> X { return x1+x2; };
	auto fa = [](X x, M m) -> X { return x+m; };
	auto fm = [](M m1, M m2) -> M { return m1+m2; };
	auto fp = [](M m, ll n_) -> M { return m*n_; };
	X ex = 0;
	M em = 0;
	LazySegmentTree<X, M> seg(n, fx, fa, fm, fp, ex, em);
	for(i = 0; i < n; i++) seg.Set(i, i+1);
	seg.Build();  // {1, 2, 3, 4, 5, 6};
	assert(seg.Query(0, 4) == 10);
	assert(seg.Query(0, 6) == 21);
	assert(seg.Query(5, 6) == 6);
	seg.Update(1, 3, -3);  // {1, -1, 0, 4, 5, 6};
	assert(seg.Query(0, 6) == 15);
	seg.Update(0, 6, 0);  // {1, -1, 0, 4, 5, 6};
	assert(seg.Query(0, 6) == 15);
	seg.Update(0, 1, 5);  // {6, -1, 0, 4, 5, 6};
	assert(seg.Query(0, 2) == 5);
	vector<int> a = {6, -1, 0, 4, 5, 6};
	for(i = 0; i < n; i++)
	{
		assert(seg.Get(i) == a[i]);
	}
}

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F&lang=ja
void Test_AOJ_DSL_2_F(void)
{
	int n, q;
	cin >> n >> q;
	using X = int;
	using M = int;
	auto fx = [](X x1, X x2) -> X { return min(x1, x2); };
	auto fa = [](X x, M m) -> X { return m; };
	auto fm = [](M m1, M m2) -> M { return m2; };
	X ex = numeric_limits<X>::max();
	M em = numeric_limits<M>::max();
	LazySegmentTree<X, M> seg(n, fx, fa, fm, nullptr, ex, em);

	// 初期化
	for(int i = 0; i < n; i++)
	{
		seg.Update(0, n, (1LL<<31)-1);
	}

	int c, s, t, x;
	while(q > 0)
	{
		cin >> c;
		if(c == 0)  // update
		{
			cin >> s >> t >> x;
			t++;  // 閉区間から半開区間への変換
			seg.Update(s, t, x);
		}
		else  // find
		{
			cin >> s >> t;
			t++;  // 閉区間から半開区間への変換
			cout << seg.Query(s, t) << endl;
		}
		q--;
	}	
}

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G
void Test_AOJ_DSL_2_G(void)
{
	int n, q; cin >> n >> q;
	using X = ll;
	using M = ll;
	auto fx = [](X x1, X x2) -> X { return x1+x2; };
	auto fa = [](X x, M m) -> X { return x+m; };
	auto fm = [](M m1, M m2) -> M { return m1+m2; };
	auto fp = [](M m, ll n_) -> M { return m*n_; };
	X ex = 0;
	M em = 0;
	LazySegmentTree<X, M> seg(n+1, fx, fa, fm, fp, ex, em);
	ll c, s, t, x;
	while(q > 0)
	{
		q--;
		cin >> c;
		if(c == 0)  // add
		{
			cin >> s >> t >> x;
			t++;
			seg.Update(s, t, x);
		}
		else  // getsum
		{
			cin >> s >> t;
			t++;
			cout << seg.Query(s, t) << endl;
		}
	}
}

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_H
void Test_AOJ_DSL_2_H(void)
{
	int n, q; cin >> n >> q;
	using X = int;
	using M = int;
	auto fx = [](X x1, X x2) -> X { return min(x1, x2); };
	auto fa = [](X x, M m) -> X { return x+m; };
	auto fm = [](M m1, M m2) -> M { return m1+m2; };
	X ex = numeric_limits<X>::max();
	M em = 0;
	LazySegmentTree<X, M> seg(n, fx, fa, fm, nullptr, ex, em);
	// 初期化
	int i;
	for(i = 0; i < n; i++) seg.Set(i, 0);
	seg.Build();
	int c, s, t, x;
	while(q > 0)
	{
		cin >> c;
		if(c == 0)  // add
		{
			cin >> s >> t >> x;
			t++;  // 閉区間から半開区間への変換
			seg.Update(s, t, x);
		}
		else  // find
		{
			cin >> s >> t;
			t++;  // 閉区間から半開区間への変換
			cout << seg.Query(s, t) << endl;
		}
		q--;
	}

}

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_I&lang=ja
// RSQ and RUQ
void Test_AOJ_DSL_2_I(void)
{
	int n, q; cin >> n >> q;
	using X = ll;
	using M = ll;
	auto fx = [](X x1, X x2) -> X { return x1+x2; };  // sum
	auto fa = [](X x, M m) -> X { return m; };
	auto fm = [](M m1, M m2) -> M { return m2; };  // update
	auto fp = [](M m, ll n_) -> M { return m*n_; };  // sumのため区間に比例
	X ex = 0;
	M em = numeric_limits<M>::max();  // updateする値として取りえない値
	LazySegmentTree<X, M> seg(n, fx, fa, fm, fp, ex, em);
	ll c, s, t, x;
	while(q > 0)
	{
		q--;
		cin >> c;
		if(c == 0)  // update
		{
			cin >> s >> t >> x;
			t++;
			seg.Update(s, t, x);
		}
		else  // getsum
		{
			cin >> s >> t;
			t++;
			cout << seg.Query(s, t) << endl;
		}
	}
}

int main(void)
{
	/*
	[代表的なfx,ex等の例]
	RMQ and RUQ(Range Minimum Query and Range Update Query)
	---------------
	using X = int;
	using M = int;
	auto fx = [](X x1, X x2) -> X { return min(x1, x2); };
	auto fa = [](X x, M m) -> X { return m; };
	auto fm = [](M m1, M m2) -> M { return m2; };
	X ex = numeric_limits<X>::max();
	M em = numeric_limits<M>::max();
	LazySegmentTree<X, M> seg(n, fx, fa, fm, nullptr, ex, em);
	---------------

	RSQ and RAQ(Range Sum Query and Range Add Query)
	区間に比例した作用素が必要
	---------------
	using X = ll;
	using M = ll;
	auto fx = [](X x1, X x2) -> X { return x1+x2; };
	auto fa = [](X x, M m) -> X { return x+m; };
	auto fm = [](M m1, M m2) -> M { return m1+m2; };
	auto fp = [](M m, ll n_) -> M { return m*n_; };
	X ex = 0;
	M em = 0;
	LazySegmentTree<X, M> seg(n, fx, fa, fm, fp, ex, em);
	---------------

	RMQ and RAQ(Range Minimum Query and Range Add Query)
	---------------
	using X = int;
	using M = int;
	auto fx = [](X x1, X x2) -> X { return min(x1, x2); };
	auto fa = [](X x, M m) -> X { return x+m; };
	auto fm = [](M m1, M m2) -> M { return m1+m2; };
	X ex = numeric_limits<X>::max();
	M em = 0;
	LazySegmentTree<X, M> seg(n, fx, fa, fm, nullptr, ex, em);
	---------------

	RSQ and RUQ
	---------------
	using X = ll;
	using M = ll;
	auto fx = [](X x1, X x2) -> X { return x1+x2; };  // sum
	auto fa = [](X x, M m) -> X { return m; };
	auto fm = [](M m1, M m2) -> M { return m2; };  // update
	auto fp = [](M m, ll n_) -> M { return m*n_; };  // sumのため区間に比例
	X ex = 0;
	M em = numeric_limits<M>::max();  // updateする値として取りえない値 INF64でも可
	LazySegmentTree<X, M> seg(n, fx, fa, fm, fp, ex, em);
	---------------

	Range XOR Query(区間をXORで更新、区間のXORを答える)
	---------------
	using X = int;
	using M = int;
	auto fx = [](X x1, X x2) -> X { return x1^x2; };
	auto fa = [](X x, M m) -> X { return x^m; };
	auto fm = [](M m1, M m2) -> M { return m1^m2; };
	X ex = 0;
	M em = 0;
	LazySegmentTree<int, int> seg(N, fx, fa, fm, nullptr, ex, em);
	---------------
	*/
	Test();
	const int mode = 2;
	if(mode == 0) {
		Test_AOJ_DSL_2_F();
	}
	else if(mode == 1) {
		Test_AOJ_DSL_2_G();
	}
	else if(mode == 2) {
		Test_AOJ_DSL_2_H();
	}
	else if(mode == 3) {
		Test_AOJ_DSL_2_I();
	}
	return 0;

	return 0;
}
