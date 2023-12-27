#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC221 https://atcoder.jp/contests/abc221

/*
 * セグメント木で解く。手順は以下。
 * ・Aを大きい順、値が同じなら右側の要素を優先してアクセスしていく。
 *   実装ではa_idx[]を用意し、a[]の値を使ってソートを行った。
 * ・Aにアクセスする要素番号をidxとする。
 *   セグメント木にて [idx+1,N) の和を求め、sumとする。
 *   sum/2^(idx+1) をansに加算する。これがidx番目の要素をA'1としたときの部分列の個数となる。
 * ・セグメント木にて 2^idx をidx番目の要素に設定する。
 * 
 * 部分列のA'1とA'kの距離がxのとき、部分列は2^(x-1)だけ存在するという考え方。
 * Aの大きい方からA'1としたときの部分列の数を計算し、
 * またA'k用に2^idxをセグメント木に設定しておく。
 * 
 * 最初、セグメント木を使わずに進めてハマった。
 * [0,N)全体の総和を(modinvを使って)2^idxで割れば、idxより手前の要素は小数以下になって0になる…と考えたが、
 * MOD計算だと小数部分も残ってしまう。
 * セグメント木を使って総和の範囲を指定する必要があった。
 * 
 * また、自身のセグ木のライブラリにMOD処理を入れるのを忘れずに。
 */

// Range Sum Query(RSQ)の実装
// (1)Add(i, val) : 要素iにvalを加算する
// (2)GetSum(a, b) : 区間[a,b)にある要素の総和を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/29/204841
// https://algo-logic.info/segment-tree/
template <typename T>
struct SegmentTree_RSQ
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	vector<T> node;
	const T INITIAL = (T)0;  // 初期値

public:
	// 要素数で初期化
	SegmentTree_RSQ(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INITIAL);
	}

	// 要素iにvalを加算する
	void Add(int i, T val)
	{
		i += n-1;  // node[]の要素番号に変換
		// 最下段のnodeを更新したら、あとは親へ上って更新していく
		node[i] += val;
		node[i] %= MOD;
		while(i > 0)
		{
			i = (i-1)/2;  // 親
			node[i] = node[2*i+1] + node[2*i+2];
			node[i] %= MOD;
		}
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

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = GetSum(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = GetSum(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return (vl + vr) % MOD;
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
			node[i] = node[2*i+1] + node[2*i+2];
			node[i] %= MOD;
		}
	}
};

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> a_idx(N);
	iota(a_idx.begin(), a_idx.end(), 0);
	// a[]の大きい順、同じなら右側の要素が優先
	auto srt = [&](int x, int y) -> bool
	{
		if(a[x] > a[y]) return true;
		if(a[x] == a[y] && x > y) return true;
		return false;
	};
	sort(a_idx.begin(), a_idx.end(), srt);  // a_idxの順にa[]にアクセス

	SegmentTree_RSQ<ll> seg(N);
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		auto idx = a_idx[i];
		// [idx+1,N)の和を2^(idx+1)で割ってansに加算
		ll sum = seg.GetSum(idx+1, N);
		ll m = modpow(2, idx+1, MOD);
		m = modinv(m, MOD);
		ans += sum * m;
		ans %= MOD;

		// 2^idxをセグ木に入れる
		m = modpow(2, idx, MOD);
		seg.Add(idx, m);
	}
	cout << ans << endl;

	return 0;
}
