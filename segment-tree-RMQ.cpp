#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
// const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int INF32 = (int)((1UL<<31)-1);
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
struct SegmentTree_RMQ
{
private:
	int n;
	vector<int> node;

public:
	// 元配列vをセグメント木で表現する
	SegmentTree_RMQ(vector<int> v)
	{
		int i;
		int size = v.size();
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// セグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INF32);

		// 最下段を設定
		for(i = 0; i < size; i++)
		{
			node[n-1+i] = v[i];
		}
		// それより上は、自分の子を参照  下から順に
		for(i = n-2; i >= 0; i--)
		{
			node[i] = min(node[2*i+1], node[2*i+2]);
		}
	}

	// 要素xをvalで更新する
	void Update(int x, int val)
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
	int GetMin(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値を返す
		if(r <= a || b <= l) return INF32;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		int vl = GetMin(a, b, 2*k+1, l, (l+r)/2);  // 左側
		int vr = GetMin(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return min(vl, vr);
	}
};

void Test(void)
{
	vector<int> v = {3, 1, 4, 1, 5, 9};
	SegmentTree_RMQ rmq(v);

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
	vector<int> v(n, INF32);
	SegmentTree_RMQ rmq(v);

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
