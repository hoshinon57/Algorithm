#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
#include <limits>  // numeric_limits
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC357 https://atcoder.jp/contests/abc357

/*
 * 遅延セグメント木で解く。
 * 
 * 要素に、その区間の {Aの総和, Bの総和, ΣA*B, 区間長} を持たせる。
 * Aにxが加算されると、ΣA*Bは (その区間のBの総和)*x だけ増加する。
 * Bに加算される場合も同様。
 * またlazy処理によりA,Bそれぞれに加算される状態のとき、どちらから計算しても結果は同じ。
 * 
 * A,Bについて区間和の管理が必要となるが、fpでなく要素自体に区間長を持たせることとした。
 * あとは実装を参照。
 * 
 * [ACまでの思考の流れ]
 * ・直観でセグメント木、区間に対する操作があるので遅延セグメント木になると考えた。
 * ・ΣA*Bをどう管理するか。
 *   セグメント木をA,Bで2本持たせるのはどうも上手くいかない。
 *   「Aにxを加算する」がどうΣA*Bに影響するかを考えたときに、(その区間のB総和)*x だけ増えるみたい。
 *   とすると、 Asum, Bsum, ABsum の3つを持たせれば良いのでは。
 * ・区間和なので自作のfpを用いる？
 *   いや、考察を進めると前述の *x は区間長によらず一定である必要がある。
 *   とするとfpを使わず、要素自体に区間長を持たせる形になりそう。
 * 
 * fpを使わず、要素自体に区間長を持たせるという考え方を学ぶことができた。
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
	// ※node[]アクセスに変更しようと考えたが、遅延評価が必要のためQuery()呼び出しが必須のはず
	X Get(int i) { return Query(i, i+1); }
};

struct S
{
	ll asum, bsum, absum;
	ll len;
};

int main(void)
{
	ll N, Q; cin >> N >> Q;
	using Xt = S;
	using Mt = array<ll,2>;  // [0]:Aに加算する値 [1]:Bに加算する値
	auto fx = [](Xt x1, Xt x2) -> Xt {
		// 区間長含め、単純にx1+x2
		Xt ret;
		ret.asum = (x1.asum+x2.asum) % MOD;
		ret.bsum = (x1.bsum+x2.bsum) % MOD;
		ret.absum = (x1.absum+x2.absum) % MOD;
		ret.len = x1.len + x2.len;
		return ret;
	};
	auto fa = [](Xt x, Mt m) -> Xt {
		// Aのadd分をabsumに加え、Aをadd
		x.absum += m[0] * x.bsum;
		x.absum %= MOD;
		x.asum += m[0] * x.len;
		x.asum %= MOD;
		// Bについて同様
		x.absum += m[1] * x.asum;
		x.absum %= MOD;
		x.bsum += m[1] * x.len;
		x.bsum %= MOD;
		return x;
	};
	auto fm = [](Mt m1, Mt m2) -> Mt {
		Mt ret;
		ret[0] = (m1[0]+m2[0])%MOD;
		ret[1] = (m1[1]+m2[1])%MOD;
		return ret;
	};
	Xt ex = S{0, 0, 0, 0};
	Mt em = {0, 0};
	LazySegmentTree<Xt, Mt> seg(N+5, fx, fa, fm, nullptr, ex, em);

	ll i;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	for(i = 0; i < N; i++)
	{
		seg.Set(i, S{a[i], b[i], a[i]*b[i]%MOD, 1});
	}
	seg.Build();
	
	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			ll l, r, x; cin >> l >> r >> x;
			l--; r--;
			r++;  // [l,r)
			seg.Update(l, r, {x, 0});
		}
		else if(k == 2)
		{
			ll l, r, x; cin >> l >> r >> x;
			l--; r--;
			r++;  // [l,r)
			seg.Update(l, r, {0, x});
		}
		else
		{
			ll l, r; cin >> l >> r;
			l--; r--;
			r++;  // [l,r)
			auto t = seg.Query(l, r);
			cout << t.absum << endl;
		}
	}

	return 0;
}
