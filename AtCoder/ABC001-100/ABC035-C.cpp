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

// ABC035 https://atcoder.jp/contests/abc035

/*
 * 区間加算、区間和の遅延セグメント木で解く。
 * 
 * [l,r]のコマを裏返すことを、[l,r]の区間に1を加算することとして考えると、
 * ある面の値が偶数なら黒、奇数なら白となる。
 * これを遅延セグメント木で処理すればよい。
 * 
 * ある面xの値は、GetSum(x, x+1)で取得できる。
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

int main(void)
{
	int N, Q;
	cin >> N >> Q;
	LazySegmentTree_RAQ_RSQ<int> seg(N);

	while(Q > 0)
	{
		int l, r;
		cin >> l >> r;
		l--; r--;  // 0-indexedへの変換
		r++;  // 半開区間への変換
		seg.Add(l, r, 1);
		Q--;
	}

	for(int i = 0; i < N; i++)
	{
		int s = seg.GetSum(i, i+1);
		if(s%2 == 0) cout << "0";
		else cout << "1";
	}
	cout << endl;

	return 0;
}
