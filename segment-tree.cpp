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
 * ABC125-C
 * ABC140-E
 * ABC157-E
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
 * 典型90-37
 */

// (1)Update(x, val) : 要素xをvalで更新する
// (2)Query(a, b) : 区間[a,b)にある要素のモノイド積を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Get(i) : 要素iを取得する
// (5)Find_Leftmost(a, b, x) : 区間[a,b)の範囲で、x以下となる最も左側の要素番号を返す
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

	// Find_Leftmost(),Find_Rightmost()のテスト
	{
		vector<int> a = {3, 1, 4, 1, 5, 9, 2};
		SegmentTree<int> seg2(a.size(), fx, ex);
		for(int i = 0; i < (int)a.size(); i++)
		{
			seg2.Set(i, a[i]);
		}
		seg2.Build();
		assert(seg2.Find_Leftmost(0, 7, 1) == 1);
		assert(seg2.Find_Leftmost(1, 7, 1) == 1);
		assert(seg2.Find_Leftmost(2, 7, 1) == 3);
		assert(seg2.Find_Leftmost(0, 7, 0) == 7);
		assert(seg2.Find_Leftmost(2, 3, 4) == 2);
		assert(seg2.Find_Leftmost(2, 3, 1) == 3);
		assert(seg2.Find_Leftmost(0, 7, 10) == 0);
		assert(seg2.Find_Rightmost(0, 7, 1) == 3);
		assert(seg2.Find_Rightmost(0, 4, 1) == 3);
		assert(seg2.Find_Rightmost(0, 3, 1) == 1);
		assert(seg2.Find_Rightmost(0, 7, 0) == -1);
		assert(seg2.Find_Rightmost(0, 7, 10) == 6);
	}
	{
		vector<int> a = {1, 1, 1};
		SegmentTree<int> seg2(a.size(), fx, ex);
		for(int i = 0; i < (int)a.size(); i++)
		{
			seg2.Set(i, a[i]);
		}
		seg2.Build();
		assert(seg2.Find_Leftmost(0, 3, 1) == 0);
		assert(seg2.Find_Leftmost(1, 3, 1) == 1);
		assert(seg2.Find_Leftmost(2, 3, 1) == 2);
		assert(seg2.Find_Leftmost(0, 2, 1) == 0);
		assert(seg2.Find_Leftmost(1, 3, 1) == 1);
		assert(seg2.Find_Leftmost(0, 3, -1) == 3);
		assert(seg2.Find_Rightmost(0, 3, 1) == 2);
		assert(seg2.Find_Rightmost(2, 3, 1) == 2);
		assert(seg2.Find_Rightmost(0, 1, 1) == 0);
		assert(seg2.Find_Rightmost(0, 3, -1) == -1);
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
	Test();

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
