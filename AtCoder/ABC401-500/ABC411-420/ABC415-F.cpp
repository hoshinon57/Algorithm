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

// ABC415 https://atcoder.jp/contests/abc415

/*
 * セグメント木に載せるものを頑張るやつ。
 * 類題：ABC322-F(Vacation Query)
 * 
 * セグ木にて以下のように区間を管理する。
 *   ・ln,lc: 左から同じ文字が何文字連続するか、とその文字
 *   ・rn,rc: 右から同様
 *   ・mx: その区間におけるf(t). いわゆる答。
 *     左右端からではなく中央に連続最大長があるケースもあるので、必要。
 *   ・len: いま見ている区間の長さ。
 *     セグ木に区間長を持たせる[典型]
 * 
 * クエリは上記のmxを出力すればよい。
 * 二項演算fx(x1,x2)を頑張る必要がある。
 * 
 * まず、単位元exのlenを0にしておき、
 * fxにて片方がexであればもう片方を返してしまう、という実装が楽。[典型]
 * 
 * x1,x2の境界が一致するケースなら、mid=x1.rn+x2.lnとしておいて
 * mx=(x1.ln, x2.rn, mid) で確定する。
 * 
 * 注意点として、x1,x2の境界が一致するケースのとき、
 * x1が"aaaa"など全て同じ文字のときはx1.lnを調整必要。これは前述のmidに等しくなる。
 * x2側も同様。
 * 
 * 後は実装を参照。
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
	// max_right()と違ってRange Sum Queryにも対応でき、また非再帰なので高速。
	// ただし色々な二項演算に対する動作確認は不十分であり、使用時は注意。
	// (range max/minならばmax_right()の方が安心かも)
	// [verify]ACLPC_J, ABC392-F
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

#if 1
	// デバッグ出力 [i,i+width)でiはstepごと増加、全体は[l,r)でQueryを呼び出していく
	// debug(1, (好みの値)) とかで呼び出すのが多そうか
	// 要素の型によってはcoutできないものもあるため、ビルドエラー防止で#if 0としている
	void debug(int step, int width, int l = -1, int r = -1)
	{
		return;
		if(l == -1) { l = 0; r = n; }  // 引数無しは全区間
		cout << "debug:[" << l << "," << r << ")" << endl;
		for(int i = l; i < r; i += step)
		{
			auto dd = this->Query(i, i+width);
			cout << " [" << i << "," << i+width << "):";
			cout << dd.lc << " " << dd.ln << " " << dd.rc << " " << dd.rn << " " << dd.mx << " " << dd.len << endl;
//			cout << " [" << i << "," << i+width << "):" << this->Query(i, i+width) << endl;
		}
	}
#endif
};

struct dt
{
	char lc; ll ln;
	char rc; ll rn;
	ll mx;  // 長さMAX
	ll len;  // 区間長
};

int main(void)
{
	using T = dt;
	auto fx = [](T x1, T x2) -> T {
		// exの場合
		if(x1.len == 0) return x2;
		if(x2.len == 0) return x1;

		T ret;
		ll mid = 0;
		if(x1.rc == x2.lc)  // x1,x2の境界が一致する
		{
			mid = x1.rn + x2.ln;
		}
		ret.mx = max({x1.mx, x2.mx, mid});
		ret.len = x1.len + x2.len;
		ret.lc = x1.lc;
		ret.ln = x1.ln;
		ret.rc = x2.rc;
		ret.rn = x2.rn;

		if(x1.rc == x2.lc)  // もう一度
		{
			if(x1.ln == x1.len)  // x1が"aaaa"など全て同じ文字
			{
				ret.ln = mid;  // x1,x2境界部分で計算したものを利用
			}
			if(x2.rn == x2.len)
			{
				ret.rn = mid;
			}
		}

		return ret;
	};
	T ex = {'*', 0, '*', 0, 0, 0};

	ll i;
	ll N, Q; cin >> N >> Q;
	string s; cin >> s;
	SegmentTree<T> seg(N+5, fx, ex);
	for(i = 0; i < N; i++)
	{
		seg.Set(i, {s[i], 1, s[i], 1, 1, 1});
	}
	seg.Build();

	while(Q > 0)
	{
		Q--;
		int ki; cin >> ki;
		if(ki == 1)
		{
			ll pos; cin >> pos;
			char c; cin >> c;
			pos--;
			seg.Update(pos, {c, 1, c, 1, 1, 1});
		}
		if(ki == 2)
		{
			ll l, r; cin >> l >> r;
			l--; r--;  // [l,r]
			r++;  // [l,r)
			auto d = seg.Query(l, r);
			cout << d.mx << endl;
		}
	}

	return 0;
}
