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

// ABC354 https://atcoder.jp/contests/abc354

/*
 * セグメントツリーでLISを求めて解く方法。
 *   https://atcoder.jp/contests/abc354/editorial/10027
 *   https://x.com/kyopro_friends/status/1791829441703981096
 *   https://ikatakos.com/pot/programming_algorithm/dynamic_programming/longest_common_subsequence
 * 
 * まず、Aを座標圧縮しておく。
 * セグ木の要素iを「LISにてAiまで使ったとき、左から何番目か」として使う。
 * iの昇順に操作していき、各iについて[0,Ai)の最大値+1が、AiをLISに使うときの長さとなる。
 * これがセグ木を用いたLISの構築方法。
 * (セグ木の全区間のQueryがLISの長さ)
 * 
 * 上記を右側からも操作して、「LISにてAiが右から何番目か」も求めておき、
 * 左右の和+1がLISの長さであれば条件を満たす。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・セグ木を用いたLISの構築方法、および「それが何を意味しているか」を覚えておく。
 */

// (1)Update(x, val) : 要素xをvalで更新する
// (2)Query(a, b) : 区間[a,b)にある要素のモノイド積を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Get(i) : 要素iを取得する
// (5)Find_Leftmost(a, b, x) : 区間[a,b)の範囲で、x以下となる最も左側の要素番号を返す
// (6)debug(step, width, l, r) : デバッグ出力
// [注意]
//   0-indexed, および半開区間で処理する。
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

	// [注意]
	// 以下のFind_Leftmostは、x以下のものを探すのであれば(現状のソースコード)、Range Minのfx,exを指定すること。
	// x以上のものを探したい場合、
	//   Range "MAX"のfx,exを指定
	//   関数内を1か所修正：node[k] > x を node[k] < x へ変更
	// とすること。

	// [a,b)の範囲で、x以下となる最も左側の要素番号を返す
	// 範囲内にx以下が見つからなければ、b(=範囲外)を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	int Find_Leftmost(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 自分の値がxより大きい   もしくは
		// クエリ[a,b)と対象[l,r)が交わらない
		if(node[k] > x || (r <= a || b <= l)) return b;  // 自身の右隣を返す

//		if(k >= n-1) return k-(n-1);  // 自分が葉なら、その位置を返す
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

	// [注意]Find_Leftmost()側のコメント参照

	// [a,b)の範囲で、x以下となる最も右側の要素番号を返す
	// 範囲内にx以下が見つからなければ、a-1(=範囲外)を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	int Find_Rightmost(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 自分の値がxより大きい   もしくは
		// クエリ[a,b)と対象[l,r)が交わらない
		if(node[k] > x || (r <= a || b <= l)) return a-1;  // 自身の右隣を返す

//		if(k >= n-1) return k-(n-1);  // 自分が葉なら、その位置を返す
		if(k >= n-1) return l;  // 自分が葉なら、その位置を返す
		// 葉なので、lが位置を表している

		int vr = Find_Rightmost(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		if(vr != a-1)  // 右側に答がある
		{
			return vr;
		}
		else
		{
			return Find_Rightmost(a, b, x, 2*k+1, l, (l+r)/2);  // 左側
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
		return;
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

void solve(void)
{
	using T = int;
	auto fx = [](T x1, T x2) -> T { return max(x1, x2); };
	T ex = 0;

	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	auto ac = compression_one<int>(a);
	SegmentTree<T> sl(N, fx, ex), sr(N, fx, ex);
	vector<int> pl(N), pr(N);
	// LISでi番目の要素を使うとき、左から何番目か
	for(i = 0; i < N; i++)
	{
		int tmp = sl.Query(0, ac[i]) + 1;
		sl.Update(ac[i], tmp);
		pl[i] = tmp;
	}
	// LISでi番目の要素を使うとき、右から何番目か
	for(i = N-1; i >= 0; i--)
	{
		int tmp = sr.Query(ac[i]+1, N) + 1;  // ac[i]+1にしないと、Aiとして同じ値があったときにNG
		sr.Update(ac[i], tmp);
		pr[i] = tmp;
	}
	int len = sl.Query(0, N);  // LISの長さ

	vector<int> ans;
	for(i = 0; i < N; i++)
	{
		if(pl[i] + pr[i] - 1 == len) ans.push_back(i+1);
	}
	int sz_ = (int)ans.size();
	cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}

	return 0;
}
