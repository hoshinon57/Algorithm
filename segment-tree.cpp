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
 * ・区間上の値を更新する
 * ・任意の区間における最小値や合計値を取得する
 * といった処理をO(logN)でできるデータ構造。
 * 要素には任意のモノイドを用いることができる(抽象化)。
 * 
 * SegmentTree:
 *   以下の操作をO(logN)で処理できる。
 *   (1)Update(x, val) : 要素xをvalで更新する
 *   (2)Query(a, b) : 区間[a,b)にある要素のモノイド積を返す
 *   (3)max_right(a, b, f), min_left(a, b, f) : セグ木上の二分探索 参考実装：Test_ACLPC_J()
 *   0-indexed, および半開区間で処理する。
 *   コンストラクタには (size:要素数, fx_:二項演算, ex_:単位元) を指定する。
 *   ★代表的なfx,exはmain()に記述している。
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
 *   https://tsutaj.hatenablog.com/entry/2017/03/29/204841
 * 
 * [関連する問題 / verifyした問題]
 * AOJ DSL_2_A https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A&lang=ja
 * https://atcoder.jp/contests/practice2/tasks/practice2_j  max_rightのverify
 * ABC038-D inline DP
 * ABC125-C
 * ABC140-E
 * ABC157-E
 * ABC186-F
 * ABC223-F
 * ABC231-F
 * ABC254-F GCD
 * ABC276-F
 * ABC283-F セグ木4本
 * ABC285-F
 * ABC306-F
 * ABC331-F
 * ABC334-F
 * ABC339-E
 * ABC341-E
 * ABC343-F
 * ABC351-F 平面走査
 * ABC353-G
 * ABC354-F LIS
 * ABC369-F 平面走査
 * ABC405-F 平面走査
 * ABC406-F オイラーツアー
 * ABC408-F 平面走査
 * 典型90-37
 */

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

void Test(void)
{
	using T = int;
	auto fx = [](T x1, T x2) -> T { return min(x1, x2); };
	T ex = numeric_limits<T>::max();
	vector<int> v = {3, 1, 4, 1, 5, 9};
	SegmentTree<T> seg(v.size(), fx, ex);
	for(int i = 0; i < (int)v.size(); i++)
	{
		seg.Set(i, v[i]);
	}
	seg.Build();

	assert(seg.Query(0, 6) == 1);
	assert(seg.Query(0, 3) == 1);
	assert(seg.Query(3, 4) == 1);
	assert(seg.Query(4, 5) == 5);
	seg.Update(6, -1);
	seg.Update(2, 0);
	assert(seg.Query(0, 7) == -1);
	assert(seg.Query(1, 4) == 0);
	for(int i = 0; i < (int)v.size(); i++)
	{
		assert(seg.Get(i) ==  seg.Query(i, i+1));
	}
}

void Test_max_right(void)
{
	using T = int;
	auto fx = [](T x1, T x2) -> T { return max(x1, x2); };
	T ex = numeric_limits<T>::min();
	vector<int> a = {3, 1, 4, 1, 5, 9, 2};
	//               0  1  2  3  4  5  6
	int N = (int)a.size();
	SegmentTree<T> seg(N, fx, ex);
	for(int i = 0; i < N; i++)
	{
		seg.Set(i, a[i]);
	}
	seg.Build();

	auto lmd = [](T x) -> bool
	{
		return x <= 4;
	};
	// 4以下の最右
	assert(seg.max_right(0, N, lmd) == 4);  // [0,4)が4以下
	assert(seg.max_right(1, N, lmd) == 4);
	assert(seg.max_right(3, N, lmd) == 4);
	assert(seg.max_right(4, N, lmd) == 4);  // [4,4)が4以下
	assert(seg.max_right(5, N, lmd) == 5);
	assert(seg.max_right(6, N, lmd) == 7);  // [6,7)が4以下
	assert(seg.max_right(2, 2, lmd) == 2);
	assert(seg.max_right(2, 3, lmd) == 3);  // [2,3)が4以下
	assert(seg.max_right(2, 4, lmd) == 4);
	assert(seg.max_right(2, 5, lmd) == 4);  // [2,4)が4以下
	assert(seg.max_right(2, 6, lmd) == 4);
}

void Test_min_left(void)
{
	using T = int;
	auto fx = [](T x1, T x2) -> T { return max(x1, x2); };
	T ex = numeric_limits<T>::min();
	vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
	//               0  1  2  3  4  5  6  7  8  9 10
	int N = (int)a.size();
	SegmentTree<T> seg(N, fx, ex);
	for(int i = 0; i < N; i++)
	{
		seg.Set(i, a[i]);
	}
	seg.Build();

	T vv;
	auto lmd= [&](T x) -> bool
	{
		return x<=vv;
	};
	// vv以下を満たす最左
	vv = 5;
	assert(seg.min_left(0, N, lmd) == 8);  // [8,N)がvv以下
	assert(seg.min_left(0, 8, lmd) == 8);  // [8,8)がvv以下、つまり閉区間
	assert(seg.min_left(0, 7, lmd) == 6);  // [6,7)
	assert(seg.min_left(0, 6, lmd) == 6);  // [6,6)
	assert(seg.min_left(0, 5, lmd) == 0);
	assert(seg.min_left(0, 4, lmd) == 0);
	assert(seg.min_left(0, 3, lmd) == 0);
	assert(seg.min_left(0, 2, lmd) == 0);
	assert(seg.min_left(0, 1, lmd) == 0);
	assert(seg.min_left(0, 0, lmd) == 0);
	vv = 10;
	assert(seg.min_left(0, N, lmd) == 0);  // [0,N)で10以下の最左 -> 全て10以下なので0
	assert(seg.min_left(0, 0, lmd) == 0);
	vv = 1;
	assert(seg.min_left(0, N, lmd) == N);  // [0,N)で1以下の最左 -> [N,N),つまり空区間
	assert(seg.min_left(0, 5, lmd) == 5);
	assert(seg.min_left(0, 4, lmd) == 3);
	assert(seg.min_left(0, 3, lmd) == 3);
	assert(seg.min_left(0, 2, lmd) == 1);
}

// https://atcoder.jp/contests/practice2/tasks/practice2_j
// セグメント木上の二分探索
void Test_ACLPC_J(void)
{
	// Range Max Query
	using T = int;
	auto fx = [](T x1, T x2) -> T { return max(x1, x2); };
	T ex = numeric_limits<T>::min();
	int N, Q; cin >> N >> Q;
	SegmentTree<T> seg(N+5, fx, ex);
	for(int i = 0; i < N; i++)
	{
		int a; cin >> a;
		seg.Set(i, a);
	}
	seg.Build();

	// クエリ3について、V > (セグ木の要素) ならtrueを返せばよい
	int vv;
	auto lmd = [&](T x) -> bool
	{
		return vv > x;
	};

	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			int x, v; cin >> x >> v;
			x--;
			seg.Update(x, v);
		}
		else if(k == 2)
		{
			int l, r; cin >> l >> r;
			l--; r--;
			r++;  // [l,r)
			cout << seg.Query(l, r) << endl;
		}
		else
		{
			int x; cin >> x >> vv;
			x--;
			int ans = seg.max_right(x, N, lmd);
			// [x,ans)の区間にてV>A, [x,ans]になるとv<=Aになる。
			// よってansがそのまま答になるが、1-indexedへ変換するため+1が必要
			cout << ans+1 << endl;
		}
	}
}

int main(void)
{
	/*
	[代表的なfx,exの例]
	Range Minimum Query(RMQ)
	---------------
	using T = int;
	auto fx = [](T x1, T x2) -> T { return min(x1, x2); };
	T ex = numeric_limits<T>::max();
	---------------

	Range Sum Query(RSQ)
	---------------
	using T = int;
	auto fx = [](T x1, T x2) -> T { return x1+x2; };
	T ex = 0;
	---------------

	Range OR Query(ABC157-E)
	---------------
	using T = int;
	auto fx = [](T x1, T x2) -> T { return x1|x2; };
	T ex = 0;
	---------------

	Range GCD Query(ABC125-C)
	---------------
	using T = ll;
	auto fx = gcd;
	T ex = 0;  // gcd(a,0)=a のため
	---------------
	*/
	/*
	特定の要素を加減算したい場合、以下のラムダ式を使うと楽かも
	ex)seg_add(seg, idx, 1);  // idx番目の要素を+1
	計算量はO(logN). seg.Get()はO(1)で作ってるのでseg.Update()の分。
	※T=pairではコンパイルエラーになるのでクラスメソッドには入れられない
	---------------
	auto seg_add = [](SegmentTree<T> &seg_, int idx_, T val_) -> void
	{
		seg_.Update(idx_, seg_.Get(idx_)+val_);
	};
	---------------
	*/
	Test();
	Test_max_right();
	Test_min_left();
	return 0;

	// 以下は AOJ DSL_2_A のもの
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A&lang=ja
	using T = int;
	auto fx = [](T x1, T x2) -> T { return min(x1, x2); };
	T ex = numeric_limits<T>::max();
	int n, q;
	cin >> n >> q;
	SegmentTree<T> seg(n, fx, ex);
	for(int i = 0; i < n; i++) seg.Set(i, (1UL<<31)-1);
	seg.Build();

	int c, x, y;
	while(q > 0)
	{
		cin >> c >> x >> y;
		if(c == 0)  // update
		{
			seg.Update(x, y);
		}
		else  // find
		{
			cout << seg.Query(x, y+1) << endl;  // 閉区間->半開区間への変換
		}
		q--;
	}

	return 0;
}
