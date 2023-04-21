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

// セグメント木のメモや実装

/*
 * [ざっくり概要]
 * ・区間上の値を更新する
 * ・任意の区間における最小値や合計値を取得する
 * といった処理をO(logN)でできるデータ構造。
 * 
 * SegmentTree_RMQ:
 *   Range Minimum Query(RMQ)をセグメント木で実装したもの。
 *   以下の操作をO(logN)で処理できる。
 *   (1)Update(x, val) : 要素xをvalで更新する
 *   (2)GetMin(a, b) : 区間[a,b)にある要素の最小値を返す
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
 *   https://tsutaj.hatenablog.com/entry/2017/03/29/204841
 *   https://algo-logic.info/segment-tree/
 * 
 * [関連する問題]
 * AOJ DSL_2_A https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A&lang=ja
 */

// Range Minimum Query(RMQ)の実装
// (1)Update(x, val) : 要素xをvalで更新する
// (2)GetMin(a, b) : 区間[a,b)にある要素の最小値を返す
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/29/204841
// https://algo-logic.info/segment-tree/
template <typename T>
struct SegmentTree_RMQ
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	vector<T> node;
	const T INF = numeric_limits<T>::max();

public:
	// 要素数で初期化
	SegmentTree_RMQ(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INF);
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
			node[x] = min(node[2*x+1], node[2*x+2]);
		}
	}

	// 区間[a,b)にある要素の最小値を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	T GetMin(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値を返す
		if(r <= a || b <= l) return INF;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = GetMin(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = GetMin(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return min(vl, vr);
	}
};

void Test(void)
{
	vector<int> v = {3, 1, 4, 1, 5, 9};
	SegmentTree_RMQ<int> rmq(v.size());
	for(int i = 0; i < (int)v.size(); i++)
	{
		rmq.Update(i, v[i]);
	}

	assert(rmq.GetMin(0, 6) == 1);
	assert(rmq.GetMin(0, 3) == 1);
	assert(rmq.GetMin(3, 4) == 1);
	assert(rmq.GetMin(4, 5) == 5);
	rmq.Update(6, -1);
	rmq.Update(2, 0);
	assert(rmq.GetMin(0, 7) == -1);
	assert(rmq.GetMin(1, 4) == 0);
}

int main(void)
{
	Test();

	// 以下は AOJ DSL_2_A のもの
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A&lang=ja
	int n, q;
	cin >> n >> q;
	SegmentTree_RMQ<int> rmq(n);
	for(int i = 0; i < n; i++) rmq.Update(i, (int)((1UL<<31)-1));

	int c, x, y;
	while(q > 0)
	{
		cin >> c >> x >> y;
		if(c == 0)  // update
		{
			rmq.Update(x, y);
		}
		else  // find
		{
			cout << rmq.GetMin(x, y+1) << endl;  // 閉区間->半開区間への変換
		}
		q--;
	}

	return 0;
}
