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

// ABC350 https://atcoder.jp/contests/abc350

/*
 * 遅延セグメント木で解いてみた版。
 * 
 * カッコの位置がそれぞれl,rにあるとき、そこを位置反転するとどこに移動するかを考えると、
 * 元の位置xに対して -x+(l+r) に移動する。(いくつか値を入れたり、計算したりすると分かる)
 * これは ax+b の1次関数を作用させることに相当し、遅延セグメント木で処理することができる。
 * 
 * 要素の値として {位置pos, 反転回数rev} を持ち、
 * 作用としては {a, b, 反転回数rev} を持たせる。
 * 作用の詳細は実装を参照。
 * ※反転回数の処理を忘れずに。
 *   0,1のトグルではなく、カウントアップさせて偶数/奇数で処理する方が楽だった。
 * 
 * 答の出力は、元の文字がどの位置に移動するかを求めてans[]に一時的に代入し、
 * ans[]の順に、英字のみ出力すれば良い。
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

// 大文字小文字を反転して返す
// cが英字以外の挙動は未確認のため注意
char chguplow(char c)
{
	if(islower(c)) return toupper(c);
	else return tolower(c);
}

int main(void)
{
	int i;
	string s; cin >> s;
	vector<pair<int,int>> kpos;  // カッコの位置{l,r}
	vector<int> stk;
	int sz = (int)s.size();
	for(i = 0; i < sz; i++)
	{
		auto e = s[i];
		if(e == '(')
		{
			stk.push_back(i);
		}
		else if(e == ')')
		{
			auto l = stk.back();
			stk.pop_back();
			kpos.push_back({l, i});
		}
	}

	// 値xを要素に持ち ax+b へと変換
	// 区間和ではないためfpは不要
	using Xt = array<ll,2>;  // {pos, 反転回数rev}
	using Mt = array<ll,3>;  // {a, b, 反転回数rev} -> ax+b
	auto fx = [](Xt x1, Xt x2) -> Xt { return min(x1, x2); };  // 1点取得なので何でもよい
	auto fa = [](Xt x, Mt m) -> Xt {  // ax+b
		Xt xret;
		xret[0] = m[0] * x[0] + m[1];
		xret[1] = x[1] + m[2];
		return xret;
	};
	auto fm = [](Mt m1, Mt m2) -> Mt {
		// m1:ax+b, m2:a'x+b' として、m1の後にm2が作用する
		// a'(ax+b)+b' = aa'x + ba' + b'
		Mt mret;
		mret[0] = m1[0] * m2[0];  // aa'x
		mret[1] = m1[1] * m2[0] + m2[1];  // ba' + b'
		mret[2] = m1[2] + m2[2];
		return mret;
	};
	Xt ex = {INF64, 0};
	Mt em = {1, 0, 0};  // 1*x+0 = x
	LazySegmentTree<Xt, Mt> seg(sz+5, fx, fa, fm, nullptr, ex, em);

	for(i = 0; i < sz; i++) seg.Set(i, {i, 0});
	seg.Build();

	auto print = [&](void) -> void
	{
		string ans(sz, '_');
		for(i = 0; i < sz; i++)
		{
			auto e = s[i];
			// if(e == '(' || e == ')') continue;
			auto d = seg.Get(i);
			if(d[1]%2 == 1) e = chguplow(e);  // 反転回数が奇数回なら、大文字小文字を反転
			ans[d[0]] = e;
		}
		for(auto &e : ans)
		{
			if(e == '(' || e == ')' || e == '_') continue;
			cout << e;
		}
		cout << endl;
	};

	for(auto [l,r] : kpos)  // カッコの位置 閉区間[l,r]
	{
		seg.Update(l, r+1, Mt{-1, l+r, 1});  // -x+(l+r) の1次関数 反転回数を+1
	}
	print();

	return 0;
}
