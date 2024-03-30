#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
#include <limits>  // numeric_limits
#include <array>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC306 https://atcoder.jp/contests/abc306

/*
 * 主客転倒とセグメントツリーを用いて解く。
 * 以下、1-indexedで記載する。
 * 
 * f(Si,Sx)で、Siのj番目の要素(=Aij)について考えてみると、
 *   Sx内にAijより小さい値がX個
 *   Si内にAijより小さい値がY個 = これは(j-1)個
 * であり、(X+1)+(j-1)番目となる。
 * 
 * 少し範囲を広げて、SxではなくΣSk としてみると(ただしSiとSは問題文中のi<kを満たすもの)、
 * Σf(Si,Sk)にてSiのj番目の要素は
 *   ΣS内にAijより小さい値の個数 X
 *   Si内にAijより小さい値がY個 = これは(N-i)*(j-1)個
 * であり、X+(N-i)+(N-i)*(j-1) となる。
 * これをAの小さい方から順に、全てのSijについて求めて、その総和が答となる。
 * 
 * ※(N-i)の加算は、Skが左側にx個ある場合、自分の順番は+1必要で、
 *   Skのkの種類は i+1～N のN-i種類あるため。
 * ※全てのSijはN*M=10^6であり、間に合う。
 * 
 * Xは区間和のセグ木にて求められる。
 * セグ木の要素iを「これまでにSiが登場した回数」とすればよい。
 * 
 * 公式解説ではAについて座標圧縮が必要と書かれているが、
 * array={A, i, j}の形で持ちソートすることで、座標圧縮は不要とした。
 * 
 * [ACまでの思考の流れ]
 * ・数え上げなので主客転倒かな？と考えた。
 * ・O(NM)系が間に合うので、各Aijごとに計算できるか考えてみる。
 * ・「小さい方から何番目か？」の問題なので、Aijが小さい順に見ていく。
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
	// 1-indexed
	int i, j, k;
	int N, M; cin >> N >> M;
	using T = int;
	auto fx = [](T x1, T x2) -> T { return x1+x2; };  // Range Sum Query
	T ex = 0;
	// セグ木で、Aの昇順に見ていったときに、現時点でSiが何回登場したかを管理
	SegmentTree<T> seg(N+5, fx, ex);
	vector<array<int,3>> arr(N*M);  // arr[i*M+j] = {Aij, i, j}  Aijでソート
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++)
		{
			int a; cin >> a;
			arr[i*M+j] = {a, i+1, j+1};  // 1-indexed
		}
	}
	sort(arr.begin(), arr.end());

	ll ans = 0;
	for(k = 0; k < N*M; k++)  // arr[k]
	{
		// i,j:Siのj文字目
		i = arr[k][1];
		j = arr[k][2];

		// Σf(Si,Sk)について考える (i<k)
		ll val = seg.Query(i+1, N+1);  // これまでのS[i+1]～S[N]の登場回数の和
		val += (ll)(N-i);  // 左にSkがあるなら自分はその+1番目なので

		// Siとしてj番目なら、左にj-1個ある
		// それをS(i+1)～SNそれぞれについて計算
		val += (ll)(N-i)*(j-1);

		ans += val;
		seg.Update(i, seg.Get(i)+1);  // 登場カウント更新
	}
	cout << ans << endl;

	return 0;
}
