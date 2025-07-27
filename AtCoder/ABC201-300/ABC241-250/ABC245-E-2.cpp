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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC245 https://atcoder.jp/contests/abc245

/*
 * 以下を参考に、セグ木の二分探索＆座標圧縮で解いてみた版。
 *   https://qiita.com/recuraki/items/248b416266337e45af3f
 * 
 * まず、縦横まとめて、チョコ/箱まとめて座標圧縮しておく。
 * そして {縦, kind(チョコ=0, 箱=1), 横} でチョコと箱を合わせて登録し、昇順ソートしておく。
 * これにより縦順、縦が同じならチョコが優先でソートされることになり、
 * ある箱を見るとき、すでに出たチョコは縦方向については箱に入る条件を満たすことになる。[典型]
 * 
 * 区間和のセグ木にて、要素に横座標を割り当て、「この横幅のチョコの数」を管理する。
 * ある箱を見るとき、箱の横幅以下でなるべく横幅が大きいチョコを選ぶのが最善なので、
 * 箱の横幅=xとすると、[0,x+1)で右端を固定し、sum=0を満たす最左を求めて、その1つ左の要素となる。
 * ([l,x+1)がsum=0を満たす最左のlとすると、(l-1)にチョコがある)
 * これはセグ木の二分探索で解ける。
**/

// (1)Update(x, val) : 要素xをvalで更新する
// (2)Query(a, b) : 区間[a,b)にある要素のモノイド積を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Get(i) : 要素iを取得する
// (5)max_right(a, b, f), max_right_2(a, b, f) :
//    [a,b)の範囲で、aを左端としてf(node)=trueとなる最右の要素番号を返す
//    max_right()はRange Max/Minぐらいにしか使えない実装になっており、注意
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
	// [注意]実装が悪く、二項演算fxがRange Max/Minぐらいしか使えない。
	// Range Sumなどはmax_right_2()の方を使うこと。
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
	// [注意]実装が悪く、二項演算fxがRange Max/Minぐらいしか使えない。
	// Range Sumなどはmin_left_2()の方を使うこと。
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

	// max_right()の非再帰版。
	// 機能説明はそちらのコメントを参照。呼び出し時の引数も同じ。
	// 
	// max_right()と違ってRange Sum Queryにも対応できる。
	// ただし色々な二項演算に対する動作確認は不十分であり、使用時は注意。
	// (range max/minならばmax_right()の方が安心かも)
	// [verify]ACLPC_J, ABC392-F
	// 
	// 参考：
	//   https://qiita.com/daris755/items/82e48e42e4f6cbc65a79
	//   https://rsk0315.hatenablog.com/entry/2020/11/25/193834
	int max_right_2(int a, int b, function<bool(T)> f) {
		int l = a, r = a;  // [l,r)は条件を満たす、とする
		int w = 1;  // 今アクセスしているnode[k]のサイズ
		int k = l+(n-1);  // node[k]
		T now = ex;  // 現在の評価値
		while(r < b && w > 0) {  // r=bになったら右端に到達したので終了
			T tmp = fx(now, node[k]);
			if(f(tmp) && r+w <= b) {  // 今見ているnode[k]を入れても条件を満たす、かつはみ出さない
				now = tmp;
				r += w;  // OKとなる区間が伸びる
				if(k%2 == 1) {  // 左側の子にいるとき
					k++;  // 右のnode[k]へ
				}
				else {  // 右側の子にいるとき
					k /= 2;  // 親の、右へ k=((k-1)/2)+1 から変形できる
					w *= 2;  // 親なのでnode[k]のサイズは倍になる
				}
			}
			else {  // 今見ているnode[k]を入れたらダメ
				// kを左の子へ rは更新しない
				k = k*2+1;
				w /= 2;  // 子なのでnode[k]のサイズは半分になる
			}
		}
		return r;
	}

	// min_left()の非再帰版。
	// 機能説明はそちらのコメントを参照。呼び出し時の引数も同じ。
	// 
	// min_left()と違ってRange Sum Queryにも対応できる。
	// ただし色々な二項演算に対する動作確認は不十分であり、使用時は注意。
	// (range max/minならばmin_left()の方が安心かも)
	// [verify] まだ
	// 
	// 参考：
	//   https://qiita.com/daris755/items/82e48e42e4f6cbc65a79
	//   https://rsk0315.hatenablog.com/entry/2020/11/25/193834
	int min_left_2(int a, int b, function<bool(T)> f) {
		int l = b, r = b;  // [l,r)は条件を満たす、とする
		int w = 1;  // 今アクセスしているnode[k]のサイズ
		int k = (r-1)+(n-1);  // node[k]
		T now = ex;  // 現在の評価値
		while(a < l && w > 0) {  // l=aになったら左端に到達したので終了
			T tmp = fx(now, node[k]);
			if(f(tmp) && l-w >= a) {  // 今見ているnode[k]を入れても条件を満たす、かつはみ出さない
				now = tmp;
				l -= w;  // OKとなる区間が伸びる
				if(k%2 == 0) {  // 右側の子にいるとき
					k--;  // 左のnode[k]へ
				}
				else {  // 左側の子にいるとき
					k = k/2 - 1;  // 親の、左へ (左側の子は/2で親にいける)
					w *= 2;  // 親なのでnode[k]のサイズは倍になる
				}
			}
			else {  // 今見ているnode[k]を入れたらダメ
				// kを右の子へ lは更新しない
				k = k*2+2;
				w /= 2;  // 子なのでnode[k]のサイズは半分になる
			}
		}
		return l;
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

// 1次元の座標圧縮
// a：座標圧縮したい元データ 処理途中で要素が書き換えられる点に注意(保持したい場合は参照を外す)
// 返り値：圧縮後のデータ
// 計算量はO(NlogN)
template <typename T>
vector<T> compression_one(vector<T> &a)
{
	vector<T> a_comp = a;

	// 元データをソートし、重複を削除する
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する

	// それぞれの元データが「何番目に小さいか」をlower_bound()で求める
	for(int i = 0; i < (int)a_comp.size(); i++)
	{
		a_comp[i] = lower_bound(a.begin(), a.end(), a_comp[i]) - a.begin();
	}

	return a_comp;
}

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> ald;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i]; ald.push_back(a[i]);}
	vector<ll> b(N); for(i = 0; i < N; i++) {cin >> b[i]; ald.push_back(b[i]);}
	vector<ll> c(M); for(i = 0; i < M; i++) {cin >> c[i]; ald.push_back(c[i]);}
	vector<ll> d(M); for(i = 0; i < M; i++) {cin >> d[i]; ald.push_back(d[i]);}
	auto ald_comp = compression_one(ald);
	ll mx = -1;  // 座圧後の最大値
	vector<array<ll,3>> dt;  // {横, kind, 縦}
	// 座圧した値を元の配列に戻し、またdtを構築していく
	for(i = 0; i < N; i++)
	{
		a[i] = ald_comp[i];
		b[i] = ald_comp[i+N];
		chmax(mx, a[i]);
		chmax(mx, b[i]);
		dt.push_back({a[i], 0, b[i]});
	}
	for(i = 0; i < M; i++)
	{
		c[i] = ald_comp[i+N+N];
		d[i] = ald_comp[i+N+N+M];
		chmax(mx, c[i]);
		chmax(mx, d[i]);
		dt.push_back({c[i], 1, d[i]});
	}
	sort(dt.begin(), dt.end());  // 縦、縦が同じならチョコ優先でソート

	// Range Sum Query(RSQ)
	using T = ll;
	auto fx = [](T x1, T x2) -> T { return x1+x2; };
	T ex = 0;
	SegmentTree<T> seg(mx+5, fx, ex);
	auto seg_add = [](SegmentTree<T> &seg_, int idx_, T val_) -> void
	{
		seg_.Update(idx_, seg_.Get(idx_)+val_);
	};
	auto lmd = [](T sum) -> bool
	{
		return sum == 0;
	};
	
	ll cnt = 0;
	for(auto [y, ki, x] : dt)
	{
		if(ki == 0)  // チョコ
		{
			seg_add(seg, x, 1);
		}
		if(ki == 1)
		{
			// [0,x+1) にて右端を固定し、sum=0を満たす最左を返す
			// [l,x+1) が最左とすると、segの要素(l-1)が1以上である
			auto l = seg.min_left_2(0, x+1, lmd);
			if(l == 0) continue;  // 箱に入れられるチョコが無かった
			cnt++;
			seg_add(seg, l-1, -1);
		}
	}
	YesNo(cnt == N);

	return 0;
}
