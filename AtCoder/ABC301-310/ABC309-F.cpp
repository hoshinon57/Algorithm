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

// ABC309 https://atcoder.jp/contests/abc309

/*
 * コンテスト中に解けず、解説を見た。
 * upsolveした実装は公式解説とほぼ同じ考え方。
 * 座標圧縮およびセグメントツリーを用いる。
 *   https://atcoder.jp/contests/abc309/editorial/6749
 *   https://atcoder.jp/contests/abc309/editorial/6764
 *   https://twitter.com/kyopro_friends/status/1677677339608764416
 * 
 * まずh,w,dの入力について、h<=w<=dになるように並べ替えることで、回転を考えなくて済むようにする。
 * 次にセグメント木に乗せるために、h,w,dをそれぞれ座標圧縮して0～(N-1)の範囲にする。
 * できたbox型データを、h,w,dの優先度の順に昇順ソートする。
 * 
 * 1点更新、区間最小を扱うセグメントツリーを使う。
 * いったん各hが異なると仮定したときに、hが小さいboxから見ていって、
 *   判定処理：seg[0,w)がdより小さいならば、Yes回答となる。
 *   更新処理：seg[w,w+1)がdより大きいならば、seg[w]をdで更新する。
 * とすればよい。
 * 
 * ただし上記では
 *   1 1 1 (=h w d)
 *   1 2 2
 * という入力にてYesと誤判定してしまう。
 * そのため同じhの区間 [h_start, h_end) をまず求めて、
 *   判定処理：[h_start, h_end) で判定
 *   更新処理：[h_start, h_end) で更新
 * とすればよい。
 */

struct box
{
	int h, w, d;
	box(void) {}
	box(int h_, int w_, int d_) : h(h_), w(w_), d(d_) {}
};
bool myCompare(const box &a, const box &b)
{
	if(a.h == b.h)
	{
		if(a.w == b.w)
		{
			return a.d < b.d;
		}
		else return a.w < b.w;
	}
	else return a.h < b.h;
}

// 1次元の座標圧縮
// 計算量はO(NlogN)
vector<int> compression_one(vector<int> &a)  // a:座標圧縮したい元データ
{
	vector<int> a_comp = a;

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

// Range Minimum Query(RMQ)の実装
// (1)Update(x, val) : 要素xをvalで更新する
// (2)GetMin(a, b) : 区間[a,b)にある要素の最小値を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Find_Leftmost(a, b, x) : 区間[a,b)の範囲で、x以下となる最も左側の要素番号を返す
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

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> h(N), w(N), d(N);
	for(i = 0; i < N; i++)
	{
		// 各入力について、h<=w<=dになるようにする
		int a, b, c; cin >> a >> b >> c;
		vector<int> array_tmp = {a, b, c};
		sort(array_tmp.begin(), array_tmp.end());
		h[i] = array_tmp[0];
		w[i] = array_tmp[1];
		d[i] = array_tmp[2];
	}
	auto h_comp = compression_one(h);
	auto w_comp = compression_one(w);
	auto d_comp = compression_one(d);
	vector<box> b(N);
	for(i = 0; i < N; i++)
	{
		b[i] = box(h_comp[i], w_comp[i], d_comp[i]);
	}
	sort(b.begin(), b.end(), myCompare);

	SegmentTree_RMQ<int> seg(N);
	int h_start, h_end;
	h_start = 0;
	h_end = 0;
	while(h_end < N)
	{
		// hの値が同じになるような半開区間 [h_start, h_end) を求める
		h_start = h_end;
		for(i = h_start+1; i < N; i++)
		{
			if(b[i].h != b[h_start].h) break;
		}
		h_end = i;

		// 同じhについて判定処理
		for(i = h_start; i < h_end; i++)
		{
			
			// seg[0,w)がdより小さいならば、Yes回答
			if(seg.GetMin(0, b[i].w) < b[i].d)
			{
				cout << "Yes" << endl;
				return 0;
			}
		}

		// 同じhについて更新処理
		for(i = h_start; i < h_end; i++)
		{
			// seg[w,w+1)がdより大きいならば、seg[w]をdで更新する
			if(seg.GetMin(b[i].w, b[i].w+1) > b[i].d)
			{
				seg.Update(b[i].w, b[i].d);
			}
		}
	}
	cout << "No" << endl;

	return 0;
}
