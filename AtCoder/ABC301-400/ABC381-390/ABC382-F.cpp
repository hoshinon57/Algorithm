#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
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

// ABC382 https://atcoder.jp/contests/abc382

/*
 * コンテスト時に方針ミスってACできず。遅延セグメント木で解く。
 * 
 * 典型90-029とほぼ同じ。
 *   https://atcoder.jp/contests/typical90/tasks/typical90_ac
 * 
 * まず、バーはRの大きい方から処理できるようソートする。
 * Range Min, Range Updateの遅延セグメント木を用意し、
 * 要素jを「j列目について、上から見ていったときに最初にブロックがある行」とする。
 * あとは実装を参照。
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
    FA fa;  // lazyをnodeに反映させるイメージ
    FM fm;  // lazyにlazyを重ねるとどうなるか？の考え方 引数の2番目が後から来た作用となる
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

#if 1  // [ToDo]実装する
	// [a,b)の範囲で、x以下となる最も左側の要素番号を返す
	// 範囲内にx以下が見つからなければ、b(=範囲外)を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	int Find_Leftmost(int a, int b, X x, int k = 0, int l = 0, int r = -1)
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
	// ※node[]アクセスに変更しようと考えたが、遅延評価が必要のためQuery()呼び出しが必須のはず
	X Get(int i) { return Query(i, i+1); }
};

int main(void)
{
	// 1-indexed
	// RMQ and RUQ(Range Minimum Query and Range Update Query)
	using Xt = int;
	using Mt = int;
	auto fx = [](Xt x1, Xt x2) -> Xt { return min(x1, x2); };
	auto fa = [](Xt x, Mt m) -> Xt { return m; };
	auto fm = [](Mt m1, Mt m2) -> Mt { return m2; };
	Xt ex = numeric_limits<Xt>::max();
	Mt em = numeric_limits<Mt>::max();

	int i;
	int H, W, N; cin >> H >> W >> N;
	vector<array<int,4>> bl;
	for(i = 0; i < N; i++)
	{
		int r, c, l; cin >> r >> c >> l;
		bl.push_back({r, c, l, i});
	}
	sort(bl.begin(), bl.end());
	reverse(bl.begin(), bl.end());  // Rの大きい順にソート

	// seg[j]:j列目について、上から見ていったときに最初にブロックがある行
	LazySegmentTree<Xt, Mt> seg(W+5, fx, fa, fm, nullptr, ex, em);
	for(i = 0; i < W+5; i++) seg.Set(i, H+1);
	seg.Build();

	vector<int> ans(N);
	for(i = 0; i < N; i++)
	{
		auto [r,c,l,idx] = bl[i];
		// [c,c+l-1] -> [c,c+l)
		int mi = seg.Query(c, c+l);
		ans[idx] = mi-1;
		seg.Update(c, c+l, mi-1);
	}
	for(i = 0; i < N; i++) cout << ans[i] << endl;

	return 0;
}
