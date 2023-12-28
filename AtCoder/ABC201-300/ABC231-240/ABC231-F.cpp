#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC231 https://atcoder.jp/contests/abc231

/*
 * 座標圧縮とセグメント木で解く。
 * 
 * 高橋くんにプレゼントiを、青木くんにプレゼントjを渡すときに、
 *   Ai>=Aj and Bi<=Bj
 * を満たしている必要がある。
 * 
 * まず、A,Bをそれぞれ座標圧縮し、[0,N)の範囲に収める(セグメント木に乗せるため)
 * {A,B}ペアをA基準でソートする。
 * 
 * {A,B}をAの昇順に見ていった場合、
 * i番目のプレゼントを固定し、i>j となるプレゼントjを考えたときに、
 *   Ajを青木くんにプレゼントする分には、高橋くんは不満は無い(Ai以下なので)。
 *   Bj>=Bi であれば、青木くんも不満は無い。
 * となる。
 * 
 * ここでセグメント木を用いて、各プレゼントについて
 *   (1)セグメント木のBiの位置を+1する。
 *   (2)Biについて、[Bi,N)となる要素数をRSQで求める。これを解に加算する。
 * という手順を繰り返していけば答が求まる。
 * ただしAi=Ajとなる要素もあるため、同じAの値の範囲で上記(1)(2)をまとめて実施する必要がある。
 * ※解説を見たら、Ai=AjかつBi=Bjのみケアすれば良さそう。
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
			node[i] = fx(node[2*i+1], node[2*i+2]);
		}
	}
};

// 1次元の座標圧縮
// a：座標圧縮したい元データ 処理途中で要素が書き換えられる点に注意(保持したい場合は参照を外す)
// 返り値：圧縮後のデータ
// 計算量はO(NlogN)
template <typename T>
vector<T> compression_one(vector<T> &a)
{
	vector<T> a_comp = a;

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

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<pair<ll,ll>> ab(N);
	vector<ll> a_(N); for(i = 0; i < N; i++) {cin >> a_[i];}
	vector<ll> b_(N); for(i = 0; i < N; i++) {cin >> b_[i];}
	a_ = compression_one(a_);
	b_ = compression_one(b_);
	for(i = 0; i < N; i++)
	{
		ab[i].first = a_[i];
		ab[i].second = b_[i];
	}
	sort(ab.begin(), ab.end());

	using T = ll;
	auto fx = [](T x1, T x2) -> T { return x1+x2; };
	T ex = 0;
	SegmentTree<ll> seg(N, fx, ex);

	ll ans = 0;
	for(i = 0; i < N; )  // iの更新はループ末尾で
	{
		int st, ed;  // [st,ed)の半開区間でAが同一
		st = i;
		for(j = st+1; j < N; j++) if(ab[j].first != ab[st].first) break;
		ed = j;

		for(j = st; j < ed; j++)
		{
			ll tmp = seg.Query(ab[j].second, ab[j].second+1);
			seg.Update(ab[j].second, tmp+1);  // Biの位置をインクリメント
		}
		for(j = st; j < ed; j++)
		{
			ll tmp = seg.Query(ab[j].second, N);  // Bi以上の要素をカウント
			ans += tmp;
		}
		i = ed;
	}
	cout << ans << endl;

	return 0;
}
