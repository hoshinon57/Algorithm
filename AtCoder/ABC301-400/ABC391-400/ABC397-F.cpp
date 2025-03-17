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

// ABC397 https://atcoder.jp/contests/abc397

/*
 * コンテスト時に解けず、解説を見た。
 * 遅延セグメント木を用いて解く。「DPをセグメント木で高速化」の典型。
 * 
 * 細かい実装やDPの持ち方は異なるが、各種解説と同じ考え方。
 *   https://atcoder.jp/contests/abc397/editorial/12457
 *   https://x.com/kyopro_friends/status/1900907191932252246
 * 
 * dp[n][k]を以下のように定義する。
 *   a[n]までで左と真ん中の数列2個を作り、
 *   そのうちa[0]～a[k]までで左側を、a[k+1]～a[n]までで真ん中の数列を作るときの、
 *   数列ごと「含まれる整数の種類数の和」の和。
 * 
 * また右側の数列について、cr[n]を
 *   a[n]までで左と真ん中の数列2個を作るときの、
 *   右側の数列に含まれる整数の種類数の和。(a[n+1]～a[N-1]で右側の数列を作ることになる)
 * と定義する。
 * 
 * 上記の定義に基づいて、各nについて
 *   max(dp[n][0]～dp[n][n-1]) + cr[n]
 * がa[n]までで2つ、a[n+1]以降で1つ数列を作るときの種類数の最大値となる。
 * よってn全体について、この最大値が答となる。
 * 
 * dp[n][k]は遅延セグメント木を用いてinlineで更新していく。
 * nを1つ進めるとき、a[n]の値が最後に出た場所をprevとすると、
 * prevより前で区切る場合は種類数が増えない。
 * よって[prev,i-1]の範囲を+1する。
 * 各iについてdp[i][i]は、値nの出現回数cnt[]を用いて計算していった。
**/

// (1)Update(a, b, x) : 区間[a,b)の要素をxを用いて更新する
// (2)Query(a, b) : 区間[a,b)にある要素にfxを作用させた値を返す
// (3)max_right(a, b, f) : [a,b)の範囲で、aを左端としてf(node)=trueとなる最右の要素番号を返す
// (4)min_left(a, b, f)  : [a,b)の範囲で、bを右端としてf(node)=trueとなる最左の要素番号を返す
// (5)debug(step, width, l, r) : デバッグ出力
// [注意]
//   0-indexed, および半開区間で処理する。
//   要素に配列を乗せる場合、vectorだとなぜか遅い。arrayか生配列(をstructでラップ)を使うこと。
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
	int max_right(int a, int b, function<bool(X)> f, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		Evaluate(k, l, r);

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
	int min_left(int a, int b, function<bool(X)> f, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		Evaluate(k, l, r);

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
	int i;
	int N; cin >> N;
	const int AMX = 300000;  // a[i]最大
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<int> cr(N);  // cr[i]:a[i]までを使って左側2つの列を作るとき、3つ目の数列における種類数
	{
		vector<int> cnt(AMX+1);  // cnt[n]:値nの出現回数
		int k = 0;
		for(i = N-1; i >= 0; i--)  // cr[i]を求める
		{
			if(i == N-1) {cr[i]=0; continue;}
			cnt[a[i+1]]++;
			if(cnt[a[i+1]] == 1) k++;
			cr[i] = k;
		}
	}

	// range max range add
	using Xt = int;
	using Mt = int;
	auto fx = [](Xt x1, Xt x2) -> Xt { return max(x1, x2); };
	auto fa = [](Xt x, Mt m) -> Xt { return x+m; };
	auto fm = [](Mt m1, Mt m2) -> Mt { return m1+m2; };
	// Xt ex = numeric_limits<Xt>::max();
	Xt ex = 0;
	Mt em = 0;
	LazySegmentTree<Xt, Mt> seg(N+5, fx, fa, fm, nullptr, ex, em);

	int ans = 0;
	int k = 0;
	vector<int> cnt(AMX+1);  // cnt[n]:値nの出現回数
	vector<int> pr(AMX+1);  // pr[n]:値nの前回の登場位置
	for(i = 0; i < N; i++)  // a[i]までを使って左側2個の数列を作る
	{
		// [ pr[a[i]], i-1 ] を+1
		seg.Update(pr[a[i]], i, 1);
		pr[a[i]] = i;

		// [i,i] を +k
		cnt[a[i]]++;
		if(cnt[a[i]] == 1) k++;
		seg.Update(i, i+1, k);

		int tmp = seg.Query(0, N);
		chmax(ans, tmp+cr[i]);
	}
	cout << ans << endl;

	return 0;
}
