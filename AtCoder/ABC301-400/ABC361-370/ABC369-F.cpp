#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
#include <limits>  // numeric_limits
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC369 https://atcoder.jp/contests/abc369

/*
 * 自力で解けず、Twitterの「セグ木で解いた」というツイートやsnuke氏の実装を見たりした。
 * 平面走査の考え方、とのこと。
 *   https://atcoder.jp/contests/abc369/submissions/57337651
 * 
 * コインを上から順、同じ行なら左から順に見ていく。
 * すると座標(r,c)にあるコインを取る際、それが最大で何枚目であるかは
 *   列が[0,c+1)の範囲にて取れる最大の枚数+1
 * となる。
 * 
 * よってセグ木のi番目の要素として、
 *   列iにてコインを取ったとして、{それまでに取れるコインの最大枚数, 最後に取ったコインの番号}
 * という情報を乗せれば上手くいく。
 * 最後に取った～は、出力時の復元に必要。
 * また、pre_idx[i]に、i番目のコインを取るとして、その手前に取るべきコインの番号 として定義しておく。
 * 
 * コインを最後まで見ながらセグ木を更新していくと、
 * seg.Query(0, W+1) にて最大枚数と最後に取ったコイン番号が得られる。
 * 後者とpre_idxを使って経路を復元していく。
 * 詳細は実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・セグ木系は思い浮かんだが、遅延セグ木で実装を進めてしまい、また復元の方針も立たなかった。
 *   セグ木には「コインを取得した部分のみ」値を更新すれば十分であった。
 * ・経路復元として、最後の地点的な要素はきっと簡単に分かるので、そこから手前へ戻っていく方針を考える。
 *   pre[idx]といった形で、取ったコインの手前をたどれるようにする手法を覚えておく。
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

int main(void)
{
	using T = pair<int,int>;  // {コイン枚数, 最後に取ったコインの番号}
	auto fx = [](T x1, T x2) -> T { return max(x1, x2); };
	T ex = {0, -1};
	// -----
	int i;
	int H, W, N; cin >> H >> W >> N;
	vector<pair<int,int>> rc(N);  // {r, c};
	for(auto &[r,c] : rc) {cin >> r >> c; r--; c--;}
	sort(rc.begin(), rc.end());
	SegmentTree<T> seg(W+5, fx, ex);

	vector<int> pre_idx(N, -1);  // pre[i]:i番目のコインを取るとき、その手前に取るべきコインの番号 -1はこのコインが1枚目
	for(i = 0; i < N; i++)
	{
		auto [r,c] = rc[i];
		auto [val, idx] = seg.Query(0, c+1);
		val++;
		pre_idx[i] = idx;
		seg.Update(c, {val, i});
	}

	// 経路復元
	string ans;
	// 番号がpre->nxtの順にコインを取るとき、その経路をansへ格納
	auto mov = [&](int pre, int nxt) -> void
	{
		int y1, x1;
		if(pre == -1) x1 = y1 = 0;  // スタート地点
		else
		{
			x1 = rc[pre].second;
			y1 = rc[pre].first;
		}
		int y2, x2;
		if(nxt == -1) {y2 = H-1; x2 = W-1;}  // ゴール地点
		else
		{
			x2 = rc[nxt].second;
			y2 = rc[nxt].first;
		}
		while(x1<x2) {ans.push_back('R'); x1++;}
		while(y1<y2) {ans.push_back('D'); y1++;}
	};
	auto [val, idx] = seg.Query(0, W+1);
	mov(idx, -1);  // 最後のコインからゴールへ
	while(idx != -1)
	{
		int pre = pre_idx[idx];
		mov(pre, idx);
		idx = pre;
	}
	reverse(ans.begin(), ans.end());  // 末尾から復元したため
	cout << val << endl;
	cout << ans << endl;

	return 0;
}
