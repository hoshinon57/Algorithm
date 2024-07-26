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
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC285 https://atcoder.jp/contests/abc285

/*
 * セグメント木で解く。考え方は公式解説と同じだった。
 *   https://atcoder.jp/contests/abc285/editorial/5514
 *   https://qiita.com/recuraki/items/38a850c4aad563853149
 * 
 * [ACまでの思考の流れ]
 * クエリ2でYesとなる条件を考えると、以下2つを満たせば良さそう。
 * (1)S[l:r]がアルファベット順に並んでいる
 * (2)S[l:r]に含まれる文字種のうち、最左(aに近い)と最右(zに近い)をそれぞれkl,krとすると、
 *    (kl+1)～(kr-1)までの文字種の登場回数が、いずれもS[l:r]とS[0:N-1]とで等しい。
 * 
 * よって上記を区間で判定できるようなデータ構造をセグ木で持たせる。
 * fxは実装を参照。
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

struct dt
{
	array<int,26> cnt{};  // cnt[i]:i番目のアルファベットの登場回数
	int kl, kr;  // cnt[i]!=0であるiのうち、最左と最右
	bool valid;  // アルファベット順に並んでいるか
};

int main(void)
{
	int i;
	int N; cin >> N;

	using T = dt;
	auto fx = [](T x1, T x2) -> T
	{
		T ret;
		if(x1.kl == -1) return x2;  // 単位元扱い
		if(x2.kl == -1) return x1;

		if(!x1.valid || !x2.valid)  // どちらかが非validなら、結合しても非valid
		{
			ret.valid = false;
			// valid=falseのときもkl,kr,cntの設定は必要。S[0:N-1]の計算に必要なため。
		}
		else
		{
			ret.valid = (x1.kr <= x2.kl);  // 結合してvalidの条件は、x1.kr<=x2.kl
		}
		ret.kl = x1.kl;
		ret.kr = x2.kr;
		for(int _ = 0; _ < 26; _++) ret.cnt[_] = x1.cnt[_] + x2.cnt[_];
		return ret;
	};
	T ex;
	ex.kl = -1;  // 単位元扱い
	SegmentTree<T> seg(N+5, fx, ex);

	auto make = [&](char c) -> T
	{
		T ret;
		int k = c - 'a';
		ret.cnt[k] = 1;
		ret.kl = ret.kr = k;
		ret.valid = true;
		return ret;
	};

	string s; cin >> s;
	for(i = 0; i < N; i++)
	{
		T tmp = make(s[i]);
		seg.Set(i, tmp);
	}
	seg.Build();

	int Q; cin >> Q;
	T tot = seg.Query(0, N);  // 全体
	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			int x; char c;
			cin >> x >> c;
			x--;
			T tmp = make(c);
			seg.Update(x, tmp);
			tot = seg.Query(0, N);  // 全体更新
		}
		else
		{
			int l, r; cin >> l >> r;
			l--; r--;  // [l,r]
			r++;  // [l,r)
			T tmp = seg.Query(l, r);
			// (tmp.kl+1)から(tmp.kr-1)の範囲のcntをチェックし、totと一致すればOK
			bool ok = true;
			if(tmp.valid)
			{
				for(i = tmp.kl+1; i <= tmp.kr-1; i++)
				{
					if(tmp.cnt[i] != tot.cnt[i]) ok = false;
				}
			}
			else ok = false;  // そもそもアルファベット順に並んでいないのでNG
			YesNo(ok);
		}
	}

	return 0;
}
