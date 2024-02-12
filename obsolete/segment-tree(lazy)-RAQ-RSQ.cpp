#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>  // numeric_limits
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 遅延セグメント木のメモや実装

/*
 * [ざっくり概要]
 * ・任意の区間の値を更新する(区間更新), 一律加算する(区間加算)
 * ・任意の区間における最小値や合計値を取得する
 * といった処理をO(logN)でできるデータ構造。
 * 
 * LazySegmentTree_RAQ_RSQ:
 * Range Add Query(RAQ)およびRange Sum Query(RSQ)をセグメント木で実装したもの。
 *   以下の操作をO(logN)で処理できる。
 *   (1)Add(a, b, x) : 区間[a,b)の要素にxを加算する
 *   (2)GetSum(a, b) : 区間[a,b)にある要素の総和を返す
 *   0-indexed, および半開区間で処理する。
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
 *   https://tsutaj.hatenablog.com/entry/2017/03/30/224339
 *   https://algo-logic.info/segment-tree/
 * 
 * [関連する問題]
 * AOJ DSL_2_G https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G&lang=ja
 * ABC035-C
 * square869120Contest#2-H https://atcoder.jp/contests/s8pc-2
 * JOI 2008年春合宿 - Typhoon
 * ABC153-F
 */

// Range Add Query(RAQ)およびRange Sum Query(RSQ)の実装
// (1)Add(a, b, x) : 区間[a,b)の要素にxを加算する
// (2)GetSum(a, b) : 区間[a,b)にある要素の総和を返す
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/30/224339
// https://algo-logic.info/segment-tree/
template <typename T>
struct LazySegmentTree_RAQ_RSQ  // 区間加算、区間和
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	vector<T> node;  // 値配列
	vector<T> lazy;  // 遅延配列
	vector<bool> lazyFlag;  // 遅延配列に値が設定されたらtrue
	const T INITIAL = (T)0;  // 初期値

	// k番目のnodeについて遅延評価を行う
	void Evaluate(int k, int l, int r)
	{
		// 以下3つの観点で実施する
		//   自ノードの値配列に値を伝播させる
		//   子ノードの遅延配列に値を伝播させる
		//   自分のノードの遅延配列を空にする
		if(lazyFlag[k])
		{
			node[k] += lazy[k];
			if(r-l > 1)  // 最下段でなければ、子へ伝搬させる
			{
				lazy[2*k+1] += lazy[k]/2;
				lazy[2*k+2] += lazy[k]/2;
				lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
			}
			lazy[k] = INITIAL;
			lazyFlag[k] = false;
		}
	}

public:
	// 要素数で初期化
	LazySegmentTree_RAQ_RSQ(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INITIAL);
		lazy.resize(2*n-1, INITIAL);
		lazyFlag.resize(2*n-1, false);
	}

	// 区間[a,b)の要素にxを加算する
	void Add(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// 非遅延セグ木と違い、最上段から設定していくイメージ

		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// [memo]
		// クエリと対象が交わらない場合でも、評価は実施必要
		// 自身(=子)の親の計算にて、最後に子のnode[]を加算しているため、
		// lazy[]からnode[]へ値を反映させておく必要がある
		Evaluate(k, l, r);

		// クエリ[a,b)と対象[l,r)が交わらないので、何もしない
		if(r <= a || b <= l) return;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b)
		{
			lazy[k] += (r-l) * x;  // 区間和なので、kが担当する要素数のx倍を加算
			lazyFlag[k] = true;
			Evaluate(k, l, r);
			return;
		}

		// 左右の子について再帰的に探索
		Add(a, b, x, 2*k+1, l, (l+r)/2);  // 左側
		Add(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		node[k] = node[2*k+1] + node[2*k+2];
	}

	// 区間[a,b)にある要素の総和を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	T GetSum(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値を返す
		if(r <= a || b <= l) return INITIAL;

		Evaluate(k, l, r);

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = GetSum(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = GetSum(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return vl + vr;
	}
};


void Test(void)
{
	LazySegmentTree_RAQ_RSQ<int> seg(6);  // [0,6)
	seg.Add(0, 2, 10);  // {10, 10,  0,  0, 0, 0}
	seg.Add(1, 4, 20);  // {10, 30, 20, 20, 0, 0}
	assert(seg.GetSum(0, 3) == 60);
	assert(seg.GetSum(0, 6) == 80);
	seg.Add(0, 6, 5);  //  {15, 35, 25, 25, 5, 5}
	assert(seg.GetSum(4, 6) == 10);
	assert(seg.GetSum(0, 6) == 110);
}

int main(void)
{
	Test();

	// 以下は AOJ DSL_2_G のもの
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G&lang=ja
	int n, q;
	cin >> n >> q;
	LazySegmentTree_RAQ_RSQ<ll> seg(n);

	int c, s, t, x;
	while(q > 0)
	{
		cin >> c;
		if(c == 0)  // add
		{
			cin >> s >> t >> x;
			s--; t--;  // 0-indexedへの変換
			t++;  // 閉区間から半開区間への変換
			seg.Add(s, t, x);
		}
		else  // getSum
		{
			cin >> s >> t;
			s--; t--;  // 0-indexedへの変換
			t++;  // 閉区間から半開区間への変換
			cout << seg.GetSum(s, t) << endl;
		}
		q--;
	}

	return 0;
}
