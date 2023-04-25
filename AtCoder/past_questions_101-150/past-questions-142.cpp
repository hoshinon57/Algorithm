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

// 「上級者が解くべき過去問精選 100 + 50 問」の問題142
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_H&lang=ja

// 区間最小、区間加算の遅延セグメント木を実装して解く。

// (1)Add(a, b, x) : 区間[a,b)の要素にxを加算する
// (2)GetMin(a, b) : 区間[a,b)にある要素の最小値を返す
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/30/224339
// https://algo-logic.info/segment-tree/
template <typename T>
struct LazySegmentTree_RMQ_RAQ  // 区間最小、区間加算
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	// RMQ, RAQであることから、
	// node[]には区間の最小値を、lazy[]には区間に一律で加算する値を設定するイメージ
	vector<T> node;  // 値配列
	vector<T> lazy;  // 遅延配列
	vector<bool> lazyFlag;  // 遅延配列に値が設定されたらtrue
	const T INITIAL = (T)0;  // 初期値
	const T INF = numeric_limits<T>::max();

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
				lazy[2*k+1] += lazy[k];
				lazy[2*k+2] += lazy[k];
				lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
			}
			lazy[k] = INITIAL;
			lazyFlag[k] = false;
		}
	}

public:
	// 要素数で初期化
	LazySegmentTree_RMQ_RAQ(int size)
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

		Evaluate(k, l, r);

		// クエリ[a,b)と対象[l,r)が交わらないので、何もしない
		if(r <= a || b <= l) return;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b)
		{
			lazy[k] += x;
			lazyFlag[k] = true;
			Evaluate(k, l, r);
			return;
		}

		// 左右の子について再帰的に探索
		Add(a, b, x, 2*k+1, l, (l+r)/2);  // 左側
		Add(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		node[k] = min(node[2*k+1], node[2*k+2]);
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

		Evaluate(k, l, r);

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = GetMin(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = GetMin(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return min(vl, vr);
	}
};

int main(void)
{
	int n, q;
	cin >> n >> q;
	LazySegmentTree_RMQ_RAQ<ll> seg(n);

	ll c, s, t, x;
	while(q > 0)
	{
		cin >> c;
		if(c == 0)  // add
		{
			cin >> s >> t >> x;
			t++;  // 閉区間から半開区間への変換
			seg.Add(s, t, x);
		}
		else  // find
		{
			cin >> s >> t;
			t++;  // 閉区間から半開区間への変換
			cout << seg.GetMin(s, t) << endl;
		}
		q--;
	}

	return 0;
}
