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

// 「上級者が解くべき過去問精選 100 + 50 問」の問題143
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://atcoder.jp/contests/s8pc-2/tasks/s8pc_2_h

/*
 * 遅延セグメント木を用いて解く。
 * 
 * 「区間のbitを反転」と区間和を処理できる遅延セグメントを実装する。
 * 以下の実装で、
 *   値配列node[]には区間和を、
 *   遅延配列lazy[]には、その区間のbitを反転させるかを、
 * それぞれ情報として持たせる。
 * 
 * ある区間について遅延評価(Evaluate())を行う場合、node[]は
 *   node[k] = (区間の長さ) - node[k]
 * と変化する。
 * 区間の長さは r-l で求められる。
 * 
 * 「反転の反転」は処理無しなので、lazy[]への設定は1とのxorを取るようにする。
 */

template <typename T>
struct LazySegmentTree_XOR_RSQ  // 区間のbit反転、区間和
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	// 値配列には区間和が、遅延配列には区間のbitを反転するか否か、を持つ
	// 遅延配列に値が設定されたかは、0,1で判定する
	vector<T> node;  // 値配列
	vector<T> lazy;  // 遅延配列
	const T INITIAL = (T)0;  // 初期値

	// k番目のnodeについて遅延評価を行う
	void Evaluate(int k, int l, int r)
	{
		// 以下3つの観点で実施する
		//   自ノードの値配列に値を伝播させる
		//   子ノードの遅延配列に値を伝播させる
		//   自分のノードの遅延配列を空にする
		if(lazy[k] != 0)
		{
			node[k] = (r-l) - node[k];
			if(r-l > 1)  // 最下段でなければ、子へ伝搬させる
			{
				lazy[2*k+1] ^= lazy[k];
				lazy[2*k+2] ^= lazy[k];
			}
			lazy[k] = INITIAL;
		}
	}

public:
	// 要素数で初期化
	LazySegmentTree_XOR_RSQ(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INITIAL);
		lazy.resize(2*n-1, INITIAL);
	}

	// 区間[a,b)のbitを反転する
	void Xor(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// 非遅延セグ木と違い、最上段から設定していくイメージ

		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		Evaluate(k, l, r);

		// クエリ[a,b)と対象[l,r)が交わらないので、何もしない
		if(r <= a || b <= l) return;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b)
		{
			lazy[k] ^= 1;  // この区間を反転する 「反転の反転」は処理無しなので、1とのxorを取る
			Evaluate(k, l, r);
			return;
		}

		// 左右の子について再帰的に探索
		Xor(a, b, 2*k+1, l, (l+r)/2);  // 左側
		Xor(a, b, 2*k+2, (l+r)/2, r);  // 右側
		node[k] = node[2*k+1] + node[2*k+2];  // node[]は区間和
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

int main(void)
{
	int n, Q;
	cin >> n >> Q;
	LazySegmentTree_XOR_RSQ<int> seg(n);

	while(Q > 0)
	{
		int q, l, r;
		cin >> q >> l >> r;
		if(q == 1)
		{
			seg.Xor(l, r);
		}
		else
		{
			cout << seg.GetSum(l, r) << endl;
		}

		Q--;
	}

	return 0;
}
