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

// ABC070 https://atcoder.jp/contests/abc070

/*
 * 動的計画法と累積和(or セグ木)で解く。
 * だいぶ昔の問題とは言え、黄DIFFを自力ACは嬉しかった。(まあ、今なら水DIFFとは思うが…)
 * drken氏の説明が分かりやすかった。
 *   https://drken1215.hatenablog.com/entry/2020/10/06/113600
 * 
 * あるカードがどういうときに「必要になる」かを考えてみる。
 * 例えばK=10,a=3のとき、集合に含まれる値の総和をSとすると、
 *   aを含むよい集合は、S>=10.
 *   aを除いたときにS=7,8,9になるような集合があれば、aは必要である。つまりSが[10,13)が該当。
 *   そういった集合がない、つまりS>=13であれば、aは不要。
 * こんな感じになる。
 * 
 * つまりa込みの集合について、その総和が[K,K+a)になるようなものが1つでもあれば、aは必要。
 * 1つも無ければaは不要。
 * aを除いたときに [K-a,K) となる。
 * 
 * ナップザックDPで解ける気もするが、各aiについて考えなければいけないのが厄介。
 * ここで「左右からの計算を組み合わせる」という典型を用いる。
 * dp_l[i][j]を、
 *   左からi個目の要素まで見て、 (i=0～N)
 *   選んだ要素の和がj (j=0～K)
 *   であるような組み合わせがあるか否か
 * と定義する。
 * (最初、組み合わせの場合の数で処理していたら、オーバーフローしたのかWAだった)
 * dp_rも右からで同様に定義する。
 * 
 * すると各aiについて、
 *   aiより手前までのdp_l[i-1][j]の結果のうち、「組み合わせがあるもの」について、
 *   X=[K-ai-j,K-j) 内のdp_r[N-i][X]で「組み合わせがある」ものが1つでもあれば、aiは必要。
 *   1つも無ければ、aiは不要。
 * となる。
 * ※実装では0-indexedなどで添え字に注意。上記の記載と実装とは添え字が一部異なる。
 * 
 * "X=[K-ai-j,K-j) 内のdp_r[N-i][X]" の部分について、愚直にやるとTLEする。
 * 自分は区間加算のセグ木でまずACした。
 * しかし累積和で十分だった…
 * ※昔の問題なのでメモリ制限が256MBで、セグ木でやったときにMLEになり苦戦した。
 **/

// (1)Update(x, val) : 要素xをvalで更新する
// (2)Query(a, b) : 区間[a,b)にある要素のモノイド積を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Get(i) : 要素iを取得する
// (5)max_right(a, b, f) : [a,b)の範囲で、aを左端としてf(node)=trueとなる最右の要素番号を返す
// (6)min_left(a, b, f)  : [a,b)の範囲で、bを右端としてf(node)=trueとなる最左の要素番号を返す
// (7)debug(step, width, l, r) : デバッグ出力
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

	// セグ木上の二分探索(max_right, min_left)の参考
	// 以下、セグ木上の要素をAi, クエリの値をvvとする
	// ・Ai>=vvとなる最左を求めたい、またはAi<vvとなる最右を求めたい
	//   Range Maxにて f(T x) { return x<vv; } としてmax_rightを使う (Ai<vvの間true)
	//   Range Minにて f(T x) { return x>=vv; } としてmin_leftを使う (Ai>=vvの間true)

	// [a,b)の範囲で、aを左端としてf(node)=trueとなる最右(ret)を返す。つまり[a,ret)がtrueで、[a,ret+1)がfalse.
	// b=Nとして呼び出すことが多いはず。
	// 全区間trueになる場合はret=bを返す。[a,b)=trueなので。
	// 全区間falseになる場合はret=aを返す。[a,a).
	// 
	// 左端からf(node)=falseを探していくイメージ。
	// 
	// f:セグ木の要素(型T)を引数に取り、bool型を返す関数を渡す
	// k:内部処理用。node[k]としてアクセス
	// l,r:内部処理用。nodeの[l,r)を対象とする。再帰にて変化する
	int max_right(int a, int b, function<bool(T)> f, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 今見ている区間[l,r)でf()=trueである、または
		// クエリ[a,b)と対象[l,r)が交わらない
		if(f(node[k]) || (r <= a || b <= l)) return b;  // 自身の右隣を返す

		if(k >= n-1) return l;  // 自分が葉なら、その位置を返す
		// 葉なので、lが位置を表している

		int vl = max_right(a, b, f, 2*k+1, l, (l+r)/2);  // 左側からf(node)=falseを探していく
		if(vl != b)  // 左側に答(falseとなる点)がある
		{
			return vl;
		}
		else
		{
			return max_right(a, b, f, 2*k+2, (l+r)/2, r);
		}
	}

	// [a,b)の範囲で、bを右端としてf(node)=trueとなる最左(ret)を返す。つまり[ret,b)がtrueで、[ret-1,b)がfalse.
	// a=0として呼び出すことが多いはず。
	// 全区間trueになる場合はret=aを返す。[a,b)=trueなので。
	// 全区間falseになる場合はret=bを返す。[b,b).
	// 
	// 右端からf(node)=falseを探していくイメージ。
	// 
	// f:セグ木の要素(型T)を引数に取り、bool型を返す関数を渡す
	// k:内部処理用。node[k]としてアクセス
	// l,r:内部処理用。nodeの[l,r)を対象とする。再帰にて変化する
	int min_left(int a, int b, function<bool(T)> f, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 今見ている区間[l,r)でf()=trueである、または
		// クエリ[a,b)と対象[l,r)が交わらない
		if(f(node[k]) || (r <= a || b <= l)) return a;  // 自身の左端を返す

		if(k >= n-1) return l+1;  // 自分が葉なら、その位置を返す
		// 葉なので、lが位置を表している
		// かつ、lがf(node[k])=falseとなる左端なので、l+1が目的の値となる

		int vl = min_left(a, b, f, 2*k+2, (l+r)/2, r);  // 右側からf(node)=falseを探していく
		if(vl != a)  // 右側に答(falseとなる点)がある
		{
			return vl;			
		}
		else
		{
			return min_left(a, b, f, 2*k+1, l, (l+r)/2);
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
	int i, j;
	int N, K; cin >> N >> K;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	auto f_dp = [&](vector<vector<int>> &dp) -> void
	{
		// もらうDP
		for(i = 1; i <= N; i++)
		{
			for(j = 0; j <= K; j++)
			{
				dp[i][j] = dp[i-1][j];
				// 組み合わせの場合の数でやると、オーバーフローしたのかWAだった
				// なので存在有無の0 or 1で実装した
//				if(j-a[i-1] >= 0) dp[i][j] += dp[i-1][j-a[i-1]];
				if(j-a[i-1] >= 0) dp[i][j] |= dp[i-1][j-a[i-1]];
			}
		}
	};
	vector<vector<int>> dpl(N+1, vector<int>(K+1, 0));
	dpl[0][0] = 1;
	f_dp(dpl);

	// 右から計算
	vector<vector<int>> dpr(N+1, vector<int>(K+1, 0));
	dpr[0][0] = 1;
	reverse(a.begin(), a.end());
	f_dp(dpr);
	reverse(a.begin(), a.end());

	using T = int;
	auto fx = [](T x1, T x2) -> T { return x1+x2; };
	T ex = 0;
	int ans = 0;
	for(i = 0; i < N; i++)  // a[i]について判定
	{
		SegmentTree<T> seg(K+5, fx, ex);
		for(j = 0; j <= K; j++)
		{
			seg.Set(j, dpr[N-i-1][j]);
		}
		seg.Build();
		bool must = false;  // a[i]が必要ならtrue
		for(j = 0; j <= K; j++)  // 総和j
		{
			if(dpl[i][j] == 0) continue;
			int l = max(K-a[i]-j, 0);
			int r = K-j;  // [l,r)
			if(seg.Query(l, r) > 0)
			{
				must = true;
				break;
			}
		}
		if(!must) ans++;
	}
	cout << ans << endl;

	return 0;
}
