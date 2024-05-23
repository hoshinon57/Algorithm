#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
#include <limits>  // numeric_limits
#include <cassert>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC322 https://atcoder.jp/contests/abc322

/*
 * 自力で解けず、解説を見た。
 * 遅延セグメント木を用いて解く。
 * 
 * 基本は解説の通り。
 * 「任意の区間の、1が連続する長さの最大」を求めるため、セグ木の要素として
 *   (1)区間の長さ
 *   (2)区間の左端から1が連続する長さ
 *   (3)区間の右端から1が連続する長さ
 *   (4)区間内の1が連続する長さの最大
 * の4つを管理する必要がある。
 * ※(1)の区間長は必要。「区間の全てが1 or 0である」判定ができず、要素の結合ができないため。
 *   (4)も必要。"101111101"といったケースで正しく求められないため。
 * 
 * 要素の結合は(1)～(4)をうまくミックスする。
 * 「区間の全てが1であるか否か」で処理分岐が必要。
 * 
 * 次に反転クエリについて考える。
 * 1に関する情報だけで、0/1反転後の情報はうまく作れないため、
 * 0について上記(2)～(4)の情報も合わせて管理すれば実現できる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・区間の操作、および区間の特定の計算を行うので、遅延セグメント木かなと。
 * ・要素にどういった情報を持たせればよいかを考える。
 *   区間の結合にて「左側の要素の右端と、右側の要素の左端」が合体する必要があり、
 *   そこから考えていけば良いのかなと。
 */

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

struct dt
{
	int len;  // 区間長
	int d[2][3];  // [0][*]:0に関する情報, [1][*]:1に関する情報
	              // [1][3]:{1の区間の最長, 左からの1の長さ, 右からの1の長さ}
};

int main(void)
{
	// 0-indexed
	int N, Q; cin >> N >> Q;
	using Xt = dt;
	using Mt = int;  // 反転状態 0:正転, 1:反転
	auto fx = [](Xt x1, Xt x2) -> Xt {
		Xt ret;
		ret.len = x1.len + x2.len;
		int i;
		for(i = 0; i < 2; i++)
		{
			ret.d[i][0] = max({x1.d[i][0], x2.d[i][0], x1.d[i][2]+x2.d[i][1]});
			if(x1.d[i][0] == x1.len)  // 区間が全てi
			{
				ret.d[i][1] = x1.len + x2.d[i][1];
			}
			else
			{
				ret.d[i][1] = x1.d[i][1];
			}
			if(x2.d[i][0] == x2.len)
			{
				ret.d[i][2] = x2.len + x1.d[i][2];
			}
			else
			{
				ret.d[i][2] = x2.d[i][2];
			}
		}
		return ret;
	};
	auto fa = [](Xt x, Mt m) -> Xt {
		if(m == 0) return x;  // 正転 em=0なので不要だが念のため
		swap(x.d[0], x.d[1]);
		return x;
	};
	auto fm = [](Mt m1, Mt m2) -> Mt {
		if(m2 == 0) return m1;  // Update()は常に1で呼ぶため不要だが念のため
		return 1-m1;  // 反転の反転は正転
	};
	Xt ex = {0, {{0, 0, 0}, {0, 0, 0}}};
	Mt em = 0;
	LazySegmentTree<Xt, Mt> seg(N+5, fx, fa, fm, nullptr, ex, em);
	string s; cin >> s;
	int i;
	for(i = 0; i < N; i++)
	{
		if(s[i] == '0') seg.Set(i, {1, {{1, 1, 1}, {0, 0, 0}}});
		else seg.Set(i, {1, {{0, 0, 0}, {1, 1, 1}}});
	}
	seg.Build();

	while(Q > 0)
	{
		Q--;
		int c, l, r; cin >> c >> l >> r;
		l--; r--;  // [l,r]
		if(c == 1)
		{
			seg.Update(l, r+1, 1);
		}
		else
		{
			auto ret = seg.Query(l, r+1);
			cout << ret.d[1][0] << endl;  // 1の長さの最長
		}
	}

	return 0;
}
