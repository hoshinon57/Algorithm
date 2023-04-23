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
 * SegmentTree_RSQ:
 *   Range Sum Query(RSQ)をセグメント木で実装したもの。
 *   以下の操作をO(logN)で処理できる。
 *   (1)Add(i, val) : 要素iにvalを加算する
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
 *   https://tsutaj.hatenablog.com/entry/2017/03/29/204841
 *   https://algo-logic.info/segment-tree/
 * 
 * [関連する問題]
 * AOJ DSL_2_B https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B&lang=ja
 * ABC125-C
 */

// Range Minimum Query(RSQ)の実装
// (1)Add(i, val) : 要素iにvalを加算する
// (2)GetSum(a, b) : 区間[a,b)にある要素の総和を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/29/204841
// https://algo-logic.info/segment-tree/
template <typename T>
struct SegmentTree_RSQ
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	vector<T> node;
	const T INITIAL = (T)0;  // 初期値

public:
	// 要素数で初期化
	SegmentTree_RSQ(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INITIAL);
	}

	// 要素iにvalを加算する
	void Add(int i, T val)
	{
		i += n-1;  // node[]の要素番号に変換
		// 最下段のnodeを更新したら、あとは親へ上って更新していく
		node[i] += val;
		while(i > 0)
		{
			i = (i-1)/2;  // 親
			node[i] = node[2*i+1] + node[2*i+2];
		}
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

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = GetSum(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = GetSum(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return vl + vr;
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
			node[i] = node[2*i+1] + node[2*i+2];
		}
	}
};

void Test(void)
{
	vector<int> v = {3, 1, 4, 1, 5, 9};
	SegmentTree_RSQ<int> rsq(v.size());
	for(int i = 0; i < (int)v.size(); i++)
	{
		rsq.Set(i, v[i]);
	}
	rsq.Build();

	assert(rsq.GetSum(0, 6) == 23);
	assert(rsq.GetSum(0, 3) == 8);
	assert(rsq.GetSum(3, 4) == 1);
	assert(rsq.GetSum(4, 5) == 5);
	rsq.Add(6, -1);
	rsq.Add(2, -4);
	assert(rsq.GetSum(6, 7) == -1);
	assert(rsq.GetSum(1, 4) == 2);
	assert(rsq.GetSum(0, 7) == 18);
}

int main(void)
{
	Test();

	// 以下は AOJ DSL_2_B のもの
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B&lang=ja
	int n, q;
	cin >> n >> q;
	SegmentTree_RSQ<int> rsq(n);

	int c, x, y;
	while(q > 0)
	{
		cin >> c >> x >> y;
		if(c == 0)  // add
		{
			x--;  // 0-indexedに変換
			rsq.Add(x, y);
		}
		else  // find
		{
			x--; y--;  // 0-indexedに変換
			y++;  // 閉区間->半開区間への変換
			cout << rsq.GetSum(x, y) << endl;
		}
		q--;
	}

	return 0;
}
