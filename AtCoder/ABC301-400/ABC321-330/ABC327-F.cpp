#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>  // function
#include <limits>  // numeric_limits
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC327 https://atcoder.jp/contests/abc327

/*
 * 遅延セグメント木で解く。
 * 各種解説より、「平面操作」という考え方とのこと。
 *   https://drken1215.hatenablog.com/entry/2023/11/11/010900
 *   https://twitter.com/kyopro_friends/status/1720798793610993858
 *   https://atcoder.jp/contests/abc327/editorial/7579
 * 
 * 座標xにりんごが落ちてくるとき、[x,x+W)の範囲にカゴ(の右端)を設置すればりんごを取れる。
 * つまり[x,x+W)の範囲で+1するという考え方で、0から2*10^5までの間の最大値が、りんごを取れる数の最大となる。
 * これを時刻方向にも拡張する。
 * 時刻tに+1のイベントを、時刻t+Dに-1のイベントを設置し、各時刻について処理していく。
 * 
 * [x,x+W)の範囲で+1する：区間加算
 * 0から2*10^5までの間の最大値：区間最大
 * より、遅延セグメント木で解くことができる。
 * 
 * [ACまでの思考の流れ]
 * ・座標xにりんごが落ちてくるなら、[x,x+W)に設置したカゴ(の右端)なら取れる。
 *   a[pos]を "位置posにカゴを設置したときに取れるりんごの数" と定義すると、[x,x+W)の区間に+1していくイメージ。
 *   りんごの数の最大値はa[]の最大値であり、区間最大を求める形になる。
 *   これは遅延セグ木で解けそう。
 * ・時刻方向については、
 *     りんごが落ちてくる時刻
 *     これを過ぎるともうりんごが取れなくなる時刻
 *   の2つをイベントとして用意し、各時刻について処理していけばよさそう。
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

int main(void)
{
	const ll MX = 200000+5;  // 時刻および座標の最大
	int i;
	// range max query & range add query
	ll N, D, W; cin >> N >> D >> W;
	using X = ll;
	using M = ll;
	auto fx = [](X x1, X x2) -> X { return max(x1, x2); };
	auto fa = [](X x, M m) -> X { return x+m; };
	auto fm = [](M m1, M m2) -> M { return m1+m2; };
	X ex = 0;
	M em = 0;
	LazySegmentTree<X, M> seg(MX, fx, fa, fm, nullptr, ex, em);
	// 要素は0で初期化

	// add[time][*] = {pos1, pos2, ...}
	vector<vector<ll>> add(MX), sub(MX);
	for(i = 0; i < N; i++)
	{
		ll t, x; cin >> t >> x;
		add[t].push_back(x);
		t = min(t+D, MX);
		sub[t].push_back(x);
	}
	ll ans = 0;
	for(i = 0; i < MX; i++)
	{
		for(auto &e : add[i])
		{
			ll r = min(e+W, MX);
			seg.Update(e, r, 1);
		}
		for(auto &e : sub[i])
		{
			ll r = min(e+W, MX);
			seg.Update(e, r, -1);
		}
		chmax(ans, seg.Query(0, MX));
	}
	cout << ans << endl;

	return 0;
}
