#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>  // numeric_limits
#include <map>
#include <set>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 「上級者が解くべき過去問精選 100 + 50 問」の問題144
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://atcoder.jp/contests/joisc2008/tasks/joisc2008_typhoon

/*
 * 座標圧縮と遅延セグメント木を用いて解く。
 * 
 * まず観測地点kは最大10^9であり、そのままではメモリ制限を超過する。
 *   台風の数nは10^5まで (地点数はその2倍)
 *   クエリmは10^5まで
 * なので、地点の種類は最大で3*10^5程度。
 * よって座標圧縮する。
 * 
 * 台風番号について、「台風が1つも発生しない初期状態」を番号0にすると処理がしやすいため、1-indexedにする。
 * その他の値は0-indexedとする。
 * 
 * 入力a[], b[]について、区間加算＆一点取得ができる遅延セグメント木を用いて
 *   (1)台風ごとに[a,b]を加算
 *   (2)クエリごとに地点pの "台風rが過ぎた後の値" - "台風(q-1)が過ぎた後の値" を計算
 * すればよい。
 * ※手持ちのライブラリでは区間加算＆区間取得があったので、それを使った。
 * 
 * (2)についてはクエリ先読みを行い、以下2つのオブジェクトを用意する。
 *   higai_list[台風x][*]:
 *     台風x経過後に、それまでの観測回数を求めたい地点yの一覧
 *     higai_list[x] = {y1, y2, y3, ...}
 *   higai_map[{台風x, 地点y}]:
 *     台風x経過後に、地点yにおける観測回数
 *     higai_map[{x, y}] = (観測回数)
 * 
 * higai_listをまずクエリを先読みして設定し、
 * 台風ごとにセグ木を更新する際に、higai_list[台風x]の一覧を元に
 * higai_mapを求めていく。
 * 最後に、クエリごとにhigai_mapから回答を計算すればよい。
 */

// Range Add Query(RAQ)およびRange Sum Query(RSQ)の実装
// (1)Add(a, b, x) : 区間[a,b)の要素にxを加算する
// (2)GetSum(a, b) : 区間[a,b)にある要素の総和を返す
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/30/224339
// https://algo-logic.info/segment-tree/
template <typename T>
struct LazySegmentTree_RAQ_RSQ  // 区間加算、区間和
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	vector<T> node;  // 値配列
	vector<T> lazy;  // 遅延配列
	vector<bool> lazyFlag;  // 遅延配列に値が設定されたらtrue
	const T INITIAL = (T)0;  // 初期値

	// k番目のnodeについて遅延評価を行う
	void Evaluate(int k, int l, int r)
	{
		// 以下3つの観点で実施する
		//   自ノードの値配列に値を伝播させる
		//   子ノードの遅延配列に値を伝播させる
		//   自分のノードの遅延配列を空にする
		if(lazyFlag[k])
		{
			node[k] += lazy[k];
			if(r-l > 1)  // 最下段でなければ、子へ伝搬させる
			{
				lazy[2*k+1] += lazy[k]/2;
				lazy[2*k+2] += lazy[k]/2;
				lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
			}
			lazy[k] = INITIAL;
			lazyFlag[k] = false;
		}
	}

public:
	// 要素数で初期化
	LazySegmentTree_RAQ_RSQ(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INITIAL);
		lazy.resize(2*n-1, INITIAL);
		lazyFlag.resize(2*n-1, false);
	}

	// 区間[a,b)の要素にxを加算する
	void Add(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// 非遅延セグ木と違い、最上段から設定していくイメージ

		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// [memo]
		// クエリと対象が交わらない場合でも、評価は実施必要
		// 自身(=子)の親の計算にて、最後に子のnode[]を加算しているため、
		// lazy[]からnode[]へ値を反映させておく必要がある
		Evaluate(k, l, r);

		// クエリ[a,b)と対象[l,r)が交わらないので、何もしない
		if(r <= a || b <= l) return;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b)
		{
			lazy[k] += (r-l) * x;  // 区間和なので、kが担当する要素数のx倍を加算
			lazyFlag[k] = true;
			Evaluate(k, l, r);
			return;
		}

		// 左右の子について再帰的に探索
		Add(a, b, x, 2*k+1, l, (l+r)/2);  // 左側
		Add(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		node[k] = node[2*k+1] + node[2*k+2];
	}

	// 区間[a,b)にある要素の総和を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	T GetSum(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値を返す
		if(r <= a || b <= l) return INITIAL;

		Evaluate(k, l, r);

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = GetSum(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = GetSum(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return vl + vr;
	}
};

int main(void)
{
	// 台風番号は1-indexed
	// 地点は座標圧縮する

	ll i;
	ll n, m, k;  // 台風の数n, クエリm, 地点数k
	cin >> n >> m >> k;
	vector<ll> a(n+5), b(n+5);  // a[i],b[i]:台風iにて被害を受ける範囲[a,b]
	vector<ll> point_list = {0};  // 座標圧縮用 出現した地点を登録していく
	for(i = 1; i <= n; i++)
	{
		cin >> a[i] >> b[i];
		point_list.push_back(a[i]);
		point_list.push_back(b[i]);
	}
	vector<ll> p(m), q(m), r(m);  // 地点p[i]が、台風q[i]からr[i]までの間に被害を受けた回数
	for(i = 0; i < m; i++)
	{
		cin >> p[i] >> q[i] >> r[i];
		point_list.push_back(p[i]);
	}

	// 地点を座標圧縮
	// 対象はa[], b[], p[]であり、これらを書き換える
	sort(point_list.begin(), point_list.end());
	point_list.erase(unique(point_list.begin(), point_list.end()), point_list.end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する
	for(i = 1; i <= n; i++)
	{
		a[i] = lower_bound(point_list.begin(), point_list.end(), a[i]) - point_list.begin();
		b[i] = lower_bound(point_list.begin(), point_list.end(), b[i]) - point_list.begin();
	}
	for(i = 0; i < m; i++)
	{
		p[i] = lower_bound(point_list.begin(), point_list.end(), p[i]) - point_list.begin();
	}

	ll point_size = point_list.size() + 10;
	// 台風x経過後に、それまでの観測回数を求めたい地点yの一覧
	// higai_list[x] = {y1, y2, y3, ...}
	vector<set<ll>> higai_list(n+5);  // higai_list[n+5][*]

	// 台風x経過後に、地点yにおける観測回数
	// higai_map[{x, y}] = (観測回数)
	map<pair<ll, ll>, ll> higai_map;
	for(i = 0; i < m; i++)
	{
		higai_list[q[i]-1].insert(p[i]);  // 台風Qの発生前の状態が知りたいので-1
		higai_list[r[i]  ].insert(p[i]);
	}

	LazySegmentTree_RAQ_RSQ<ll> seg(point_size);  // 地点ごとの観測回数を遅延セグ木で処理
	for(i = 1; i <= n; i++)  // 台風i
	{
		seg.Add(a[i], b[i]+1, 1);  // 半開区間にするためb[i]+1
		for(auto &e : higai_list[i])  // 地点e
		{
			higai_map[{i, e}] = seg.GetSum(e, e+1);
		}
	}

	for(i = 0; i < m; i++)
	{
		ll h1, h2;
		h1 = higai_map[{q[i]-1, p[i]}];
		h2 = higai_map[{r[i],   p[i]}];
		cout << h2 - h1 << endl;
	}

	return 0;
}
