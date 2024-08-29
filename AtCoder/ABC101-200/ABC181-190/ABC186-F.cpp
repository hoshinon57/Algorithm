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

// ABC186 https://atcoder.jp/contests/abc186

/*
 * 自力で解けず、解説をチラ見した。セグメントツリーを用いて解く。
 *   https://atcoder.jp/contests/abc186/editorial/400
 *   https://drken1215.hatenablog.com/entry/2020/12/20/111600
 *   https://fairy-lettuce.hatenadiary.com/entry/2020/12/19/224002
 * 
 * 以下、0-indexedで記載する。また入力のX,Yを反転している(直感と反対だったため)。
 * 
 * 各種解説とは少し異なる方針になったと思われる。
 * 各行ごと見ていき、「下右の順で行けるマス」「下右の順では行けず、右下の順では行けるマス」をカウントしていく。
 * 
 * gyou[y]={x1,x2,x3,...}を、y行目にある障害物のx座標とする。実装では各行に番兵を入れている。
 * 前述のカウントを以下のように計算していく。
 * (1)下右の順で行けるマス
 *   X座標が [0, gyou[y][0]) の半開区間であるマスが対象。単純にO(1)で足せばよい。
 * (2)下右の順では行けず、右下の順では行けるマス
 *   これまでの行で(今見ている行も含む)障害物があったX座標には、行けない。
 *   よってセグ木で要素iを「X=iにこれまで障害物があったかどうか」で管理すれば、
 *   [gyou[y][0], W) の半開区間にて行けるマスは求めることができる。
 * 
 * 0行目に障害物があるケースと、0列目に障害物があるケースに注意。
 * 前者は、そこより右の全マスに障害物があると処理すれば楽。
 * 後者は、そこより下の全マスに障害物があると処理すれば楽。
 * それぞれ、y0_xmin, x0_yminを参照。
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
	// 0-indexed
	// 入力とX,Y反転する
	int i;
	int H, W, M; cin >> H >> W >> M;
	vector<vector<int>> gyou(H);  // gyou[y]={x1,x2,x3,...} y行目にある障害物のx座標
	int x0_ymin = INF32;  // X=0に障害物がある行の、最も小さいYの値
	int y0_xmin = INF32;  // Y=0に障害物がある列の、最も小さいXの値
	for(i = 0; i < M; i++)
	{
		int y, x; cin >> y >> x;
		y--; x--;
		gyou[y].push_back(x);
		if(x == 0) chmin(x0_ymin, y);
		if(y == 0) chmin(y0_xmin, x);
	}
	for(i = y0_xmin+1; i < W; i++) gyou[0].push_back(i);  // 0行目に障害物がある場合、そこより右は全て障害物があるとしてしまう
	for(i = x0_ymin+1; i < H; i++) gyou[i].push_back(0);
	for(auto &e : gyou)
	{
		e.push_back(W);  // 各行に番兵
		sort(e.begin(), e.end());
	}

	using T = int;
	auto fx = [](T x1, T x2) -> T { return x1+x2; };  // RSQ
	T ex = 0;
	SegmentTree<T> seg(W+5, fx, ex);  // 要素i:これまでの行で、i列目に障害物があったなら1
	ll ans = 0;
	for(auto &e1 : gyou)
	{
		// 下右の順で行けるマス
		ans += e1[0];
		// 下右の順では行けず、右下の順では行けるマス
		for(auto &e2 : e1) seg.Update(e2, 1);  // まず、障害物がある列番号を1に
		ans += (W-e1[0]) - seg.Query(e1[0], W);  // [e1[0],W) の区間について、セグ木から引いた値が、行けるマス
	}
	cout << ans << endl;

	return 0;
}
