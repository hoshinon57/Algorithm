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

// ABC378 https://atcoder.jp/contests/abc378

/*
 * コンテスト時に解けず、解説を見た。むずい、なんだこれ。
 * セグ木、累積和などを用いて解く。
 * 
 * 今まで二重シグマは
 *   i->i+1への遷移にてAiを削除する
 * ことを考えていたが、
 *   i->i+1への遷移にてAiを追加する
 * で考えてみる。
 * 区間[l,r]でlを削除するのではなく、rを追加するイメージ。
 * 
 * 以下、AおよびAの累積和和Sについて、どちらもmod Mで考える。
 * 
 * rを固定してM*Mのマス目で考えてみると、例としてr=3のとき、
 *         A3 = S3-S2
 *      A2+A3 = S3-S1
 *   A1+A2+A3 = S3-S0
 * となる。右辺は
 *   Si-Sj = Si-Sj (Si>=Sjのとき) or Si-Sj+M (Si<Sjのとき)
 * で置き換えられる。よってrを固定したときに
 *   Sr*r - Σ(i=0～r-1)Si + (Sr<Sxの個数)*M
 * がrのときの答となる。これを1～Nまで加算すればよい。
 * 
 * Σ(i=0～r-1)Si の部分は、Sの累積和を計算しながら求められる。
 * (Sr<Sxの個数)は、セグメント木にて「要素i:累積和%M=i となる要素数」として求められる。
 * 
 * [典型]
 * ・区間の和は累積和で考える。
 * ・二重シグマにてrを固定(rを伸ばしていく)方針も考えてみる。
 * ・Mで割った余りは、「足してM以上になったらMを引く」と同じ。
 */

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
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i]; a[i]%=M;}  // mod M
	vector<ll> asum(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {asum[kk+1] = (asum[kk] + a[kk])%M;}  // mod M

	using T = ll;
	auto fx = [](T x1, T x2) -> T { return x1+x2; };
	T ex = 0;
	SegmentTree<T> seg(M, fx, ex);  // 要素i:累積和%M=i となる要素数

	// 要素idxにval加算
	auto seg_add = [](SegmentTree<T> &seg_, int idx_, T val_) -> void
	{
		seg_.Update(idx_, seg_.Get(idx_)+val_);
	};

	ll ans = 0;
	ll sumsum = 0;  // これまでの累積和の累積和
	for(i = 0; i < N; i++)  // asum[i+1]で見る
	{
		// Sr*r - Σ(i=0～r-1)Si + (Sr<Sxの個数)*M
		ll tmp1 = asum[i+1] * (i+1);
		ll tmp2 = sumsum;
		ll tmp3 = seg.Query(asum[i+1]+1, M) * M;   // Sr<Sxはasum[i+1]+1以上の要素数
		ll now = tmp1 - tmp2 + tmp3;
		ans += now;

		// asum[i+1]を+1
		// seg.Update(asum[i+1], seg.Get(asum[i+1])+1);
		seg_add(seg, asum[i+1], 1);
		sumsum += asum[i+1];
	}
	cout << ans << endl;

	return 0;
}
