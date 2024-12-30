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

// 競プロ典型90問:17 https://atcoder.jp/contests/typical90/tasks/typical90_q

/*
 * 自力で解けず、解説を見た。
 * 難易度★7の初挑戦。いやーきつかった、これが★7…
 * 余事象を考える点に加え、円環の線分については丸ごと本問を典型として覚えておこう。
 * 
 * 余事象で考える。基本は公式解説の通り。
 *   https://x.com/e869120/status/1383635232134287360
 *   https://github.com/E869120/kyopro_educational_90/blob/main/sol/017-03.cpp
 * 
 * 2つの線分(L1,R1),(L2,R2)について「端点以外で交わらない」ケースを考えてみると、
 * (1)L1,R1,L2,R2に一致がある
 * (2)R1<L2
 * (3)L1<L2<R2<L2
 * の3つがある。これらをそれぞれ求めて、全組み合わせN*(N-1)/2から引けばよい。
 * 
 * (1)は円周上の点ごとに線分の登場回数をカウントしていく。
 * ある頂点における登場回数がvであれば、v*(v-1)/2が組み合わせなので、
 * これを全頂点にて加算する。
 * O(N+M).
 * 
 * (2)はいもす法を用いた。
 * ある頂点iについてR<=iとなる個数をviとすると、これはいもす法にて各頂点のviを求めることができる。
 * そうしたら、v[l-1]を各線分について加算していけばよい。
 * O(N+M).
 * 
 * (3)はセグメント木を用いた。(公式解説ではBIT)
 * 線分を「Rの小さい順、Rが同じならLの小さい順」にソートする。
 * 先頭から見ていって、今見ている線分をnl,nrとすると、
 *   [nl+1,N)にあるLが、条件を満たすので答に加算。(図を描くと分かりやすい)
 *   セグ木の要素nlを+1する
 * という手順で求められる。
 * O(MlogN).
 * (解説を見ると、要素数Nの準備があるのでO(N+MlogN)とのこと)
 */

// (1)Update(x, val) : 要素xをvalで更新する
// (2)Query(a, b) : 区間[a,b)にある要素のモノイド積を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Get(i) : 要素iを取得する
// (5)Find_Leftmost(a, b, x) : 区間[a,b)の範囲で、x以下となる最も左側の要素番号を返す
// (6)debug(step, width, l, r) : デバッグ出力
// [注意]
//   0-indexed, および半開区間で処理する。
//   要素に配列を乗せる場合、vectorだとなぜか遅い。arrayか生配列(をstructでラップ)を使うこと。
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

#if 0
	// デバッグ出力 [i,i+width)でiはstepごと増加、全体は[l,r)でQueryを呼び出していく
	// debug(1, (好みの値)) とかで呼び出すのが多そうか
	// 要素の型によってはcoutできないものもあるため、ビルドエラー防止で#if 0としている
	void debug(int step, int width, int l = -1, int r = -1)
	{
		if(l == -1) { l = 0; r = n; }  // 引数無しは全区間
		cout << "debug:[" << l << "," << r << ")" << endl;
		for(int i = l; i < r; i += step)
		{
			cout << " [" << i << "," << i+width << "):" << this->Query(i, i+width) << endl;
		}
	}
#endif
};

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> l(M), r(M); for(i = 0; i < M; i++) {cin >> l[i] >> r[i]; l[i]--; r[i]--;}

	ll ans = 0;
	// (1)L1,L2,R1,R2に一致があるケース
	{
		vector<ll> cnt(N);
		for(i = 0; i < M; i++)
		{
			cnt[l[i]]++;
			cnt[r[i]]++;
		}
		for(i = 0; i < N; i++)
		{
			ans += (cnt[i]*(cnt[i]-1)/2);
		}
	}

	// (2)R1<L2のケース
	{
		vector<ll> imos(N+5);
		for(i = 0; i < M; i++)
		{
			imos[r[i]]++;
		}
		for(i = 0; i < N+3; i++)
		{
			imos[i+1] += imos[i];
		}
		for(i = 0; i < M; i++)
		{
			if(l[i] == 0) continue;
			ans += imos[l[i]-1];
		}
	}
	
	// (3)L1<L2<R2<L2のケース
	{
		// Range Sum Query(RSQ)
		using T = ll;
		auto fx = [](T x1, T x2) -> T { return x1+x2; };
		T ex = 0;
		SegmentTree<T> seg(N+5, fx, ex);
		vector<pair<ll,ll>> d;
		for(i = 0; i < M; i++)
		{
			d.push_back({r[i], l[i]});
		}
		sort(d.begin(), d.end());  // Rの小さい順、Rが同じならLの小さい順
		for(i = 0; i < M; i++)
		{
			auto [nr,nl] = d[i];
			ans += seg.Query(nl+1, N);
			seg.Update(nl, seg.Get(nl)+1);
		}
	}
	
	ll tot = M*(M-1)/2;
	cout << tot-ans << endl;

	return 0;
}
