#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
#include <limits>  // numeric_limits
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC140 https://atcoder.jp/contests/abc140

/*
 * 主客転倒およびセグメント木、およびセグメント木上の二分探索で解く。
 * 
 * 各p[i]について、これが2番目に大きくなるような区間を数え上げる方針。
 * 自分の実装では、「左側にp[i]より大きなものがあるようにする」ようにした。これは
 *   (1)p[i]より左で、p[i]より大きな要素の、最も右側
 *   (2)(1)より左で、p[i]より大きな要素の、最も右側
 *   (3)p[i]より右で、p[i]より大きな要素の、最も左側
 * をそれぞれ求めると、
 *   左側の選び方：(1)から(2)の手前までなので、(1)-(2)通り。
 *   右側の選び方：iから(3)の手前までなので、(3)-i通り。
 * となり、これらの積とp[i]をかけたものになる。
 * 例：
 *   P={8 2 7 3 2 2 6 1} のとき、p[3]=3について
 *   (1):p[2]
 *   (2):p[0]
 *   (3):p[6]
 *   なので、(2-0)*(6-3)*p[3] となる。
 * 
 * 「右側にp[i]より大きなものがある」ケースも必要で、
 * これはpをreverseして2ループ目を回す実装とした。
 * また先頭に番兵を入れておくことで、(2)の実装が簡易になった。
 */

// (1)Update(x, val) : 要素xをvalで更新する
// (2)Query(a, b) : 区間[a,b)にある要素のモノイド積を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Find_Leftmost(a, b, x) : 区間[a,b)の範囲で、x以下となる最も左側の要素番号を返す
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

	// [a,b)の範囲で、x以上となる最も左側の要素番号を返す
	// 範囲内にx以下が見つからなければ、b(=範囲外)を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	int Find_Leftmost(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 自分の値がxより小さい   もしくは
		// クエリ[a,b)と対象[l,r)が交わらない
		if(node[k] < x || (r <= a || b <= l)) return b;  // 自身の右隣を返す

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

	// [a,b)の範囲で、x以上となる最も右側の要素番号を返す
	int Find_Rightmost(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 自分の値がxより小さい   もしくは
		// クエリ[a,b)と対象[l,r)が交わらない
		if(node[k] < x || (r <= a || b <= l)) return a-1;  // 自身の左隣を返す

		if(k >= n-1) return l;  // 自分が葉なら、その位置を返す
		// 葉なので、lが位置を表している

		int vr = Find_Rightmost(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		if(vr != a-1)
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
};

int main(void)
{
	// 区間内のx以上の値を探すためにRange MAX Queryとする
	using T = int;
	auto fx = [](T x1, T x2) -> T { return max(x1, x2); };
	T ex = numeric_limits<T>::min();

	int i;
	int N; cin >> N;
	vector<int> p(N+2, INF32);  // 先頭はINF reverseして2ループ目も回すので、末尾もINFにしておく
	for(i = 1; i <= N; i++) cin >> p[i];

	ll ans = 0;
	for(int _ = 0; _ < 2; _++)  // reverseするので同じ処理で2ループ
	{
		SegmentTree<int> seg(N+2, fx, ex);
		seg.Set(0, INF32);  // 番兵
		seg.Set(N+1, INF32);  // 番兵
		for(i = 1; i <= N; i++) seg.Set(i, p[i]);
		seg.Build();

		for(i = 1; i <= N; i++)  // a[i]がSecondになるケースを探す
		{
			int r = seg.Find_Rightmost(0, i, p[i]+1);  // [0,i)  p[0]=INF
			if(r == 0) continue;  // iより左に、大きいものが無い
			int l = seg.Find_Rightmost(0, r, p[i]+1);  // [0,r)

			int z = seg.Find_Leftmost(i+1, N+1, p[i]+1);  // [i+1, N+1)

			ans += (ll)p[i] * (r-l) * (z-i);
		}

		reverse(p.begin(), p.end());
	}
	cout << ans << endl;

	return 0;
}
