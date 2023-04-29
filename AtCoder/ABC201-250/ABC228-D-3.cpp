#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC228 https://atcoder.jp/contests/abc228

/*
 * セグメント木で解く。
 * 
 * https://algo-logic.info/segment-tree/
 * を参考に、「ある値x以下となる最も左側の要素番号を返す」関数を用意する。
 * するとクエリt=1について、以下の手順で更新すべき要素を探せばよい。
 *   (x%N)以降で-1以下となる要素を探す
 *   見つからなかった場合、今度は先頭から探す
 */

// Range Minimum Query(RMQ)の実装
// (1)Update(x, val) : 要素xをvalで更新する
// (2)GetMin(a, b) : 区間[a,b)にある要素の最小値を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
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

		if(k >= n-1) return k-(n-1);  // 自分が葉なら、その位置を返す
		// これreturn l でもいけるのでは？

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
			node[i] = min(node[2*i+1], node[2*i+2]);
		}
	}
};

#include <cassert>
void Test(void)
{
	int i;
	{
		vector<int> a = {3, 1, 4, 1, 5, 9, 2};
		SegmentTree_RMQ<int> seg(a.size());
		for(i = 0; i < (int)a.size(); i++)
		{
			seg.Set(i, a[i]);
		}
		seg.Build();
		assert(seg.Find_Leftmost(0, 7, 1) == 1);
		assert(seg.Find_Leftmost(1, 7, 1) == 1);
		assert(seg.Find_Leftmost(2, 7, 1) == 3);
		assert(seg.Find_Leftmost(0, 7, 0) == 7);
		assert(seg.Find_Leftmost(2, 3, 4) == 2);
		assert(seg.Find_Leftmost(2, 3, 1) == 3);
		assert(seg.Find_Leftmost(0, 7, 10) == 0);
	}
	{
		vector<int> a = {1, 1, 1};
		SegmentTree_RMQ<int> seg(a.size());
		for(i = 0; i < (int)a.size(); i++)
		{
			seg.Set(i, a[i]);
		}
		seg.Build();
		assert(seg.Find_Leftmost(0, 3, 1) == 0);
		assert(seg.Find_Leftmost(1, 3, 1) == 1);
		assert(seg.Find_Leftmost(2, 3, 1) == 2);
		assert(seg.Find_Leftmost(0, 2, 1) == 0);
		assert(seg.Find_Leftmost(1, 3, 1) == 1);
		assert(seg.Find_Leftmost(0, 3, -1) == 3);
	}	
}

int main(void)
{
//	Test();

	int i;
	const int N = 1<<20;
	SegmentTree_RMQ<ll> seg(N);
	// 全要素を-1で初期化
	for(i = 0; i < N; i++)
	{
		seg.Set(i, -1);
	}
	seg.Build();

	int Q;
	cin >> Q;
	while(Q > 0)
	{
		ll t, x;
		cin >> t >> x;
		if(t == 1)
		{
			// x%N以降で、-1の要素を探す
			int idx = seg.Find_Leftmost(x%N, N, -1);
			if(idx == N)
			{
				// 見つからなかったので、先頭から再度探す
				// 問題の制約より、今度は必ず見つかることは保証できる
				idx = seg.Find_Leftmost(0, N, -1);
			}
			seg.Update(idx, x);
		}
		else
		{
			cout << seg.GetMin(x%N, x%N+1) << endl;
		}
		Q--;
	}

	return 0;
}
