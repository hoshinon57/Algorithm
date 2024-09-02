#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
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

// ABC253 https://atcoder.jp/contests/abc253

/*
 * クエリ先読みと遅延セグメント木を用いて解く。
 *   https://atcoder.jp/contests/abc253/editorial/4025
 *   https://x.com/kyopro_friends/status/1530598930844905472
 * 
 * クエリ1は遅延セグ木(解説ではBITだが)が使えそう、というのが分かる。
 * クエリ2が厄介。例えばクエリ3(i,j)の前にクエリ2(i行目更新)があれば、意味を持つのはそれ以降のクエリとなる。
 * 
 * (i,j)について、クエリ2にてどれぐらい値が変化するかを考えてみる。
 *   x:クエリ2の手前での値
 *   x':クエリ2で代入される値
 *   y:クエリ2を無視したとき、クエリ3の時点での値
 * とすると、x->x'に変化することから、出力は y+(x'-x) となる。
 * 
 * よってクエリ先読みを用いて、
 * クエリ3について、影響する直近のクエリ2がどれかを求めておく。
 * 
 * 次にクエリを再度先頭から見ていって、
 * クエリ2が来たときに、これが影響するクエリ3の番号と列を取得し、
 * クエリ2によって変化する量(前述のx->x')を計算しておく。
 * そしてクエリ3がきたら、現在の遅延セグ木の値に変化量を足したものが答になる。
 * 
 * [所管]
 * ・いやークエリ先読みは分かったが、実装が頭がこんがらがった。
 *   だいぶごちゃっとした実装になってしまったが、方向性は公式解説と同じだと思う。
 */

// (1)Update(a, b, x) : 区間[a,b)の要素をxを用いて更新する
// (2)Query(a, b) : 区間[a,b)にある要素にfxを作用させた値を返す
// [未実装](3)Find_Leftmost(a, b, x) : 区間[a,b)の範囲で、x以下となる最も左側の要素番号を返す
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/30/224339
// https://algo-logic.info/segment-tree/
template <typename X, typename M>
struct LazySegmentTree
{
private:
    using FX = function<X(X, X)>;
    using FA = function<X(X, M)>;
    using FM = function<M(M, M)>;
	using FP = function<M(M, ll)>;  // 区間和など、区間に比例した作用素用
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
    FX fx;  // モノイドX上での二項演算
    FA fa;  // lazyをnodeに反映させるイメージ
    FM fm;  // lazyにlazyを重ねるとどうなるか？の考え方 引数の2番目が後から来た作用となる
	FP fp = [](M m_, ll n_) -> M { return m_; };  // デフォルトでは区間比例は無し
	const X ex;  // モノイドX上での単位元
	const M em;  // モノイドM上での単位元 (lazyがこの値なら何も作用させないイメージ)
	vector<X> node;  // 値配列
	vector<M> lazy;  // 遅延配列

	// k番目のnodeについて遅延評価を行う
	void Evaluate(int k, int l, int r)
	{
		// 以下3つの観点で実施する
		//   子ノードの遅延配列に値を伝播させる
		//   自ノードの値配列に値を伝播させる
		//   自分のノードの遅延配列を空にする
		if(lazy[k] == em) return;  // 更新するものが無い
		if(r-l > 1)  // 最下段でなければ、子へ伝搬させる
		{
			lazy[2*k+1] = fm(lazy[2*k+1], lazy[k]);
			lazy[2*k+2] = fm(lazy[2*k+2], lazy[k]);
		}
		// 自身を更新
//		node[k] = fa(node[k], lazy[k]);
		node[k] = fa(node[k], fp(lazy[k], r-l));
		lazy[k] = em;  // lazyを空にするイメージ
	}

public:
	// 要素数で初期化
	// fp_:区間和など、区間に比例した作用が無ければnullptr
	LazySegmentTree(int size, FX fx_, FA fa_, FM fm_, FP fp_, X ex_, M em_) : fx(fx_), fa(fa_), fm(fm_), ex(ex_), em(em_)
	{
		if(fp_ != nullptr) fp = fp_;  // 引数指定があれば設定
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, ex);  // 単位元で初期化
		lazy.resize(2*n-1, em);
	}

	// 区間[a,b)の要素をxを用いて更新する
	void Update(int a, int b, M x, int k = 0, int l = 0, int r = -1)
	{
		// 非遅延セグ木と違い、最上段から設定していくイメージ

		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// [memo]
		// クエリと対象が交わらない場合でも、評価は実施必要
		// 自身(=子)の親の計算にて、最後に子のnode[]を参照しているため、
		// lazy[]からnode[]へ値を反映させておく必要がある
		Evaluate(k, l, r);

		// クエリ[a,b)と対象[l,r)が交わらないので、何もしない
		if(r <= a || b <= l) return;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b)
		{
			lazy[k] = fm(lazy[k], x);
			Evaluate(k, l, r);
			return;
		}

		// 左右の子について再帰的に探索
		Update(a, b, x, 2*k+1, l, (l+r)/2);  // 左側
		Update(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		node[k] = fx(node[2*k+1], node[2*k+2]);
	}

	// 区間[a,b)にある要素にfxを作用させた値を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	X Query(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値(=単位元)を返す
		if(r <= a || b <= l) return ex;

		Evaluate(k, l, r);

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		X vl = Query(a, b, 2*k+1, l, (l+r)/2);  // 左側
		X vr = Query(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return fx(vl, vr);
	}

#if 0  // [ToDo]実装する
	// [a,b)の範囲で、x以下となる最も左側の要素番号を返す
	// 範囲内にx以下が見つからなければ、b(=範囲外)を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	int Find_Leftmost(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		Evaluate(k, l, r);

		// 自分の値がxより大きい   もしくは
		// クエリ[a,b)と対象[l,r)が交わらない
		if(node[k] > x || (r <= a || b <= l)) return b;  // 自身の右隣を返す

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
#endif

	// 要素iをvalで更新する
	// Update()と違い、木全体の更新は行わない。Build()の呼び出しが必要。
	// 用途：初期化時に全要素を設定し、Build()で木を構築する
	void Set(int i, X val)
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
	// ※node[]アクセスに変更しようと考えたが、遅延評価が必要のためQuery()呼び出しが必須のはず
	X Get(int i) { return Query(i, i+1); }
};

int main(void)
{
	// 0-indexed
	// RMQ and RAQ(Range Minimum Query and Range Add Query)
	using Xt = ll;
	using Mt = ll;
	auto fx = [](Xt x1, Xt x2) -> Xt { return min(x1, x2); };  // 1点取得になるので、MINでもMAXでも
	auto fa = [](Xt x, Mt m) -> Xt { return x+m; };
	auto fm = [](Mt m1, Mt m2) -> Mt { return m1+m2; };
	Xt ex = numeric_limits<Xt>::max();
	Mt em = 0;
	// 	---------------
	int q;
	int N, M, Q; cin >> N >> M >> Q;
	using pii = pair<int,int>;
	LazySegmentTree<Xt, Mt> seg(M+5, fx, fa, fm, nullptr, ex, em);
	for(int i = 0; i < M; i++) seg.Set(i, 0);
	seg.Build();

	vector<int> q2idx(N, -1);  // q2idx[i]:i行目における、直近のクエリ2のクエリ番号
	vector<vector<pii>> q2_pre(Q);  // q2_pre[q]:q番目のクエリがクエリ2であるとき、 {影響するクエリ3の番号, その列j}
	// クエリ先読み
	vector<array<ll,4>> que;
	for(q = 0; q < Q; q++)
	{
		int k; cin >> k;
		if(k == 1)
		{
			ll l, r, x; cin >> l >> r >> x;
			l--; r--;
			que.push_back({k, l, r, x});
		}
		else if(k == 2)
		{
			ll i, x; cin >> i >> x;
			i--;
			que.push_back({k, i, x, -1});
			q2idx[i] = q;  // i行目における直近のクエリ番号はq
		}
		else
		{
			ll i, j; cin >> i >> j;
			i--; j--;
			que.push_back({k, i, j, -1});
			int idx = q2idx[i];
			if(idx != -1)
			{
				q2_pre[idx].push_back({q, j});  // 直近のクエリ2が来たとき、{影響するクエリ3のクエリ番号, 列j} を出せるようにしておく
			}
			else {;}  // 手前にクエリ2無し
		}
	}

	// クエリを改めて読んでいく
	vector<ll> ofs(Q);
	for(q = 0; q < Q; q++)
	{
		if(que[q][0] == 1)
		{
			auto[dm,l,r,x] = que[q];
			seg.Update(l, r+1, x);
		}
		else if(que[q][0] == 2)
		{
			auto[dm1,i,x,dm2] = que[q];
			for(auto &e : q2_pre[q])
			{
				auto[q3idx, adj] = e;
				ofs[q3idx] = x - seg.Query(adj,adj+1);
			}
		}
		else
		{
			auto[dm1,dm2,j,dm3] = que[q];
			cout << seg.Query(j, j+1) + ofs[q] << endl;
		}
	}

	return 0;
}
