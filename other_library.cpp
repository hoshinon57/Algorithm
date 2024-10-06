#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>  // numeric_limits
#include <cassert>
#include <array>
#include <random>  // rng_64
#include <chrono>  // rng_64
#include <set>
using namespace std;
typedef long long ll;

/*
 * その他のライブラリ
 * ・床関数,天井関数 floor_div, ceil_div
 * ・二分探索でidxと要素数を返す bi_idxnum_miman, 他
 * ・bビット目の確認、ビットON/OFF isbiton, setbit, unbit
 * ・桁数を返す cal_digit
 * ・p^0～p^(n-1)を事前計算 cal_pow_inadv
 * ・k乗根 kth_root_integer
 * ・2点の距離の2乗 dist_squared
 * ・乱数生成 rng_64
 * ・N*N配列を回転 rotate
 * ・Y*X配列を回転 rotate_2, rotate_2_rev
 * ・nをbase進法で表したときの値 chg_base
 * ・2つのsetをマージ(マージテク使用) set_merge
 * ・大文字小文字を反転 revLowUp
 * ・extgcd
 * ・SCC
 * ・3x3のマス目について縦/横/斜めがそろったかを判定 -> ABC349-E.cppを参照
 * ・文字列が回文かを判定 is_palindrome
 */

// a/b以下の最大の整数(床関数) floor(5,2)=2, floor(-5,2)=-3
template <typename T> T floor_div(T a, T b){ if(b<0){a=-a; b=-b;} if(a>0){return a/b;} else {return (a-b+1)/b;}}
// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val未満の最右のidx, val未満の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_miman(vector<T> &a, T val) {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {valより大きい最左のidx, valより大きい要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_koeru(vector<T> &a, T val) {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}
// {val以上の最左のidx, val以上の要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_ijou(vector<T> &a, T val)  {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

// [verify]ABC353-D
// n(>=0)の桁数を返す dig(0)=1
int cal_digit(long long n)
{
	if(n == 0) return 1;
	int c = 0;
	while(n > 0) {
		c++;
		n /= 10;
	}
	return c;
}

// [verify]ABC353-D
// p^0～p^(n-1)を事前計算して返す
// mが0以外の場合、mで割った余りを設定する
template<typename T>
vector<T> cal_pow_inadv(T p, int n, T m = 0)
{
	vector<T> ret(n);
	ret[0] = (T)1;
	for(int i = 1; i < n; i++) {
		ret[i] = ret[i-1] * p;
		if(m != 0) ret[i] %= m;
	}
	return ret;
}

// Luzhiled's Library より
// https://ei1333.github.io/library/math/number-theory/kth-root-integer.hpp.html
// aのk乗根を整数(切り捨て)で返す
// 制約：0<=a<2^64, 1<=k<=64
// 計算量：O(k*loglog(a))
// 補足：制約のk上限を外したい場合、先頭の#ifを有効化する
// #include <cstdint> が必要かも。AtCoderでは問題なかったが、Library CheckerではCEとなった。
uint64_t kth_root_integer(uint64_t a, int k) {
	if (k == 1) return a;
#if 1	// Libraryから追加 kを上限クリップしたい場合はこちらを有効
	if (a <= 1 || k == 1) return a;	// a=0時に0を返したいため、必要
	if (k >= 64) return 1;
#endif
	auto check = [&](uint32_t x) {
		uint64_t mul = 1;
		for (int j = 0; j < k; j++) {
			if (__builtin_mul_overflow(mul, x, &mul)) return false;
		}
		return mul <= a;
	};
	uint64_t ret = 0;
	for (int i = 31; i >= 0; i--) {
		if (check(ret | (1u << i))) ret |= 1u << i;
	}
	return ret;
}

// 2点(x1,y1),(x2,y2)の距離の "2乗" を返す
// 実距離を求めたい場合は呼び出し元で sqrt(dist_squared()) など行うこと
// 引数はlong longにしているが、2乗の関係で引数が10^9を超えるとオーバーフローするため注意 (10^9を超える問題はあまり出ないと思うが)
ll dist_squared(ll x1, ll y1, ll x2, ll y2) {
	ll dx = x1-x2, dy = y1-y2;
	return dx*dx+dy*dy;
}

// [使用した問題]ABC367-F
// [lo,hi]の区間の乱数を返す(閉区間)
// 必要なincludeは以下2つ
//   #include <random>
//   #include <chrono>
// 型はlong longで決め打ちしている。(mt19937_64を使っており、生成される値は64bit)
// 参考：
//   https://betrue12.hateblo.jp/entry/2019/09/07/171628
//   https://zenn.dev/mafafa/articles/f1030c3a014d4e
//   https://ei1333.github.io/luzhiled/snippets/other/random-number-generator.html
//   https://yosupo.hatenablog.com/entry/2024/06/14/064913
//   https://rsk0315.hatenablog.com/entry/2020/09/02/164428
//   https://cpprefjp.github.io/reference/random/mt19937_64.html
//   https://cpprefjp.github.io/reference/random/uniform_int_distribution.html
long long rng_64(long long lo, long long hi) {
	// static std::random_device rd;  // random_deviceを使うのは避けた方が良い環境があるらしい
	// static std::mt19937_64 rng(rd());
	static auto seed = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	static std::mt19937_64 rng(seed);
	return std::uniform_int_distribution<long long>(lo, hi)(rng);  // [lo,hi]の範囲での乱数を生成
}

// 要素がN*Nであるaについて、右に90度回転させる
void rotate(vector<vector<int>> &a)
{
	int n = a.size();
	auto a_bk = a;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			a[i][j] = a_bk[n-1-j][i];
		}
	}
}

// a[Y][X]を時計回りに90度回転させたものを返す (戻り値のサイズはa[X][Y]になる)
template <typename T>
vector<vector<T>> rotate_2(vector<vector<T>> &a)
{
	int y = (int)a.size();
	int x = (int)a[0].size();
	vector<vector<T>> a_r(x, vector<T>(y));  // a_r[x][y]
	for(int i = 0; i < y; i++)
	{
		for(int j = 0; j < x; j++)  // a[i][j]がどこに移動するか
		{
			a_r[j][y-i-1] = a[i][j];
		}
	}
	return a_r;
}

// a[Y][X]を反時計回りに90度回転させたものを返す (戻り値のサイズはa[X][Y]になる)
template <typename T>
vector<vector<T>> rotate_2_rev(vector<vector<T>> &a)
{
	int y = (int)a.size();
	int x = (int)a[0].size();
	vector<vector<T>> a_r(x, vector<T>(y));  // a_r[x][y]

	for(int i = 0; i < y; i++)
	{
		for(int j = 0; j < x; j++)  // a[i][j]がどこに移動するか
		{
			a_r[x-j-1][i] = a[i][j];
		}
	}
	return a_r;
}

// [verify]ABC336-C, ABC234-C, ABC372-B
// nをbase進法で表したときの値を返す (n=0が変換後も0に対応する)
// 戻り値は、一番下の桁から順に[0],[1],... と格納される
vector<int> chg_base(ll n, int base)
{
	if(n == 0) return {0};  // 0のときだけ特別に処理
	vector<int> a;
	while(n > 0) {
		a.push_back(n%base);
		n /= base;
	}
	return a;
}

// [verify]ABC372-E,ABC329-F
// マージテク(※)を用いて、from->toへ要素をマージする
// (※)サイズが小さい方から大きい方へマージするようにすることで、クエリ全体でO(NlogN)に抑えられる
// [注意]fromは空にはしないが、swapにより中身が変化する可能性があるため使わない方が良い
template <typename T>
void set_merge(set<T> &from, set<T> &to)
{
	if(from.size() > to.size()) swap(from, to);
	for(auto &e : from) to.insert(e);
}

// [verify]ABC350-F
// 大文字小文字を反転して返す
// cが英字以外の挙動は未確認のため注意
char revLowUp(char c) {
	if(islower(c)) return toupper(c);
	else return tolower(c);
}

// x,yについての一次不定方程式 ax+by=c が整数解を持つ条件：
//   cがgcd(a,b)で割り切れること
// よって ax+by=c の整数解を求めたい場合、以下の手順で求められる。
//   まず上記条件を確かめる (整数解を持たなければ終了)
//   以下のextgcd()にて ax+by=gcd(a,b) での{x,y}を求める
//   c/gcd(a,b) をx,yそれぞれにかける

// extgcd()にてx,yが1つ決まったら、g=gcd(a,b)として、
//   x'=x+(b/g), y'=y-(a/g)
// となる{x',y'}も式を満たす。
// 1つ求めたxがx<0のとき、(x/(b/g))+1 回だけ(b/g)を足してやれば、正である最小のxが求まる、はず。

// [verify]ABC340-F
// 一次不定方程式 ax+by=gcd(a,b) を満たす {x,y} を格納する
// 戻り値：gcd(a,b)
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/b97ff231e43bce50199a
long long extgcd(long long a, long long b, long long &x, long long &y) {
	if (b == 0) { x = 1; y = 0; return a; }
	long long d = extgcd(b, a%b, y, x);
	y -= a/b * x;
	return d;
}

/*
 * SCC(Strongly Connected Components / 強連結成分分解)のライブラリ
 * [SCCとは]
 *   有向グラフにて、互いに行き来できる全頂点を過不足なくグループ化したもの。
 *   強連結成分分解したグループを1頂点とみると、DAG(有向非巡回グラフ)となる。
 * 
 * [使い方]
 *   Graph = vector<vector<int>>; 型で有向グラフを構築する。
 *   SCC scc;
 *   vector<vector<int>> grp;
 *   vector<int> idx;
 *   scc.build(g, grp, idx);
 *   にて強連結成分分解を行う。 ※grp,idxの初期化は不要
 *   grp[i][*]:グループiに属する頂点一覧
 *   idx[i]:頂点iのグループ番号
 *   が保存される。
 * 
 *   Test_AOJ_GRL_3_C()も参照のこと。
 * 
 * [参考資料]
 *   https://manabitimes.jp/math/1250
 *   https://ei1333.github.io/luzhiled/snippets/graph/strongly-connected-components.html
 *   https://scrapbox.io/pocala-kyopro/%E5%BC%B7%E9%80%A3%E7%B5%90%E6%88%90%E5%88%86%E5%88%86%E8%A7%A3
 *   https://drken1215.hatenablog.com/entry/2023/05/06/133900
 * 
 * [関連する問題 / verifyした問題]
 *   AOJ GRL_3_C https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_C&lang=jp
 *   ABC245-F(Endless Walk) SCCしたグループにてグラフを構築する
 */

using Graph = vector<vector<int>>;
struct SCC {
private:
	vector<bool> seen;
	vector<int> order;
	// 通常の方向にDFS
	void dfs(Graph &g, int v)
	{
		if(seen[v]) return;
		seen[v] = true;
		for(auto &e : g[v]) dfs(g, e);
		order.push_back(v);  // 帰りがけ順に記録
	}
	// 逆方向にDFS
	void rdfs(Graph &rg, vector<int> &idx, int v, int gr)
	{
		if(idx[v] != -1) return;
		idx[v] = gr;
		for(auto &e : rg[v]) rdfs(rg, idx, e, gr);
	}
public:
	// 有向グラフgを強連結成分に分解し、結果をgrp,idxに保存する
	// grp[i][*]:グループiに属する頂点一覧
	// idx[i]:頂点iのグループ番号
	// grp,idxともに、関数内でまず初期化される
	void build(Graph &g, vector<vector<int>> &grp, vector<int> &idx)
	{
		int i;
		int gsz = g.size();
		// 逆方向のグラフを構築
		Graph rg(gsz);
		for(i = 0; i < gsz; i++)
		{
			// i->eの辺に対し、e->iの辺を張る
			for(auto &e : g[i]) rg[e].push_back(i);
		}

		// idx構築
		seen.assign(gsz, false);
		order.clear();
		for(i = 0; i < gsz; i++)
		{
			dfs(g, i);
		}
		reverse(order.begin(), order.end());
		int gr = 0;  // 強連結成分の数
		idx.assign(gsz, -1);
		for(auto &e : order)
		{
			if(idx[e] != -1) continue;
			rdfs(rg, idx, e, gr);
			gr++;
		}

		// grp構築
		grp.clear();
		grp.resize(gr);
		for(i = 0; i < gsz; i++)
		{
			grp[idx[i]].push_back(i);
		}

#if 0  // 縮めたグラフを構築する場合、以下のようになる [verify:ABC245-F]
		Graph shg;  // 呼び出し元から参照で渡す
		shg.clear();
		shg.resize(gr);
		for(i = 0; i < gsz; i++)
		{
			for(auto &e : g[i])  // i->e
			{
				int x = idx[i];
				int y = idx[e];
				if(x != y) shg[x].push_back(y);
			}
		}
#endif
	}
};

// SCC使用のサンプルコード
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_C&lang=jp
void Test_AOJ_GRL_3_C(void)
{
	int i;
	int V, E; cin >> V >> E;
	Graph g(V);
	for(i = 0; i < E; i++)
	{
		int s, t; cin >> s >> t;  // s->t
		g[s].push_back(t);
	}
	SCC scc;
	vector<vector<int>> grp;
	vector<int> idx;
	scc.build(g, grp, idx);

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int u, v; cin >> u >> v;
		int ans = 0;
		if(idx[u] == idx[v]) ans = 1;
		cout << ans << endl;
	}
}

// 3x3のマス目について縦/横/斜めがそろったかどうかを判定するコード
// ABC349-E.cppを参照

// 文字列sについて、stからk文字、つまり[st,st+k)の部分文字列が回文かを判定する
// 回文ならtrue
// st,kが未指定のときはs全体に対して判定する
bool is_palindrome(string &s, int st = -1, int k = -1)
{
	if(st == -1) { st = 0; k = (int)s.size(); }
	int ed = st+k-1;
	while(st <= ed)
	{
		if(s[st] != s[ed]) return false;
		st++; ed--;
	}
	return true;
}

int main(void)
{
	assert(floor_div( 5,  2) ==  2);
	assert(floor_div(-5,  2) == -3);
	assert(floor_div(-5, -2) ==  2);
	assert(floor_div( 5, -2) == -3);
	assert(floor_div( 5,  1) ==  5);
	assert(floor_div( 5, -1) == -5);
	assert(floor_div( 1000000000000000000, 60000000000) ==  16666666);  // 10^18 / 6^10
	assert(floor_div(-1000000000000000000, 30000000000) == -33333334);  // -10^18 / 3^10
	assert(ceil_div( 5,  2) ==  3);
	assert(ceil_div( 5, -2) == -2);
	assert(ceil_div(-5, -2) ==  3);
	assert(ceil_div(-5,  2) == -2);
	assert(ceil_div( 5,  1) ==  5);
	assert(ceil_div( 5, -1) == -5);
	assert(ceil_div( 1000000000000000000, 60000000000) ==  16666667);  // 10^18 / 6^10
	assert(ceil_div(-1000000000000000000, 30000000000) == -33333333);  // -10^18 / 3^10

	{
		vector<int> a = {1,3,3,4,5};
		assert(bi_idxnum_miman(a,4) == make_pair(2LL,3LL));  // 4未満の最右はa[2], 要素数は3
		assert(bi_idxnum_miman(a,0) == make_pair(-1LL,0LL));  // 0未満の最右はa[-1](該当なし), 要素数は0
		assert(bi_idxnum_miman(a,10) == make_pair(4LL,5LL));
		assert(bi_idxnum_ika(a,4) == make_pair(3LL,4LL));
		assert(bi_idxnum_ika(a,0) == make_pair(-1LL,0LL));
		assert(bi_idxnum_ika(a,10) == make_pair(4LL,5LL));
		assert(bi_idxnum_koeru(a,4) == make_pair(4LL,1LL));  // 4より大きい最左はa[4], 要素数は1
		assert(bi_idxnum_koeru(a,0) == make_pair(0LL,5LL));
		assert(bi_idxnum_koeru(a,10) == make_pair(5LL,0LL));  // 10より大きい最左はa[5](該当なし), 要素数は0
		assert(bi_idxnum_ijou(a,4) == make_pair(3LL,2LL));
		assert(bi_idxnum_ijou(a,0) == make_pair(0LL,5LL));
		assert(bi_idxnum_ijou(a,10) == make_pair(5LL,0LL));
	}

	ll x = 0b1000;
	assert(!isbiton(x, 4));
	assert( isbiton(x, 3));
	assert(!isbiton(x, 2));
	x = numeric_limits<ll>::max();
	assert( isbiton(x,  0));
	assert( isbiton(x, 62));
	assert(!isbiton(x, 63));
	x = 0;
	setbit(x, 1); assert(isbiton(x, 1)); assert(!isbiton(x, 0));
	setbit(x, 62); assert(isbiton(x, 62));
	setbit(x, 1);  // 既に立っているビット
	assert(x == ((ll)1<<62 | (ll)1<<1));
	unbit(x, 62);
	unbit(x, 62);  // 既に落ちているビット
	assert(!isbiton(x, 62));
	assert(x == ((ll)1<<1));

	assert(cal_digit(0) == 1);
	assert(cal_digit(1) == 1);
	assert(cal_digit(10) == 2);
	assert(cal_digit(999) == 3);
	assert(cal_digit(10000000000) == 11);  // 1^10は11桁

	{
		auto p = cal_pow_inadv<long long>(10, 12);
		assert(p[0] == 1);
		assert(p[1] == 10);
		assert(p[11] == (long long)1e11);
		assert(p.size() == (int)12);
		p = cal_pow_inadv<long long>(2, 11, 100);  // MOD 100
		assert(p[0] == 1);
		assert(p[6] == 64);
		assert(p[7] == 28);  // 128
		assert(p[10] == 24);  // 1024
		assert(p.size() == (int)11);
	}

	// dist_squared
	{
		assert(dist_squared(0LL, 0LL, 1LL, 2LL) == 5);
		assert(dist_squared(5LL, 10LL, 10005LL, 110LL) == 100000000+10000);
		assert(dist_squared(-10005LL, -110LL, -5LL, -10LL) == 100000000+10000);
		assert(dist_squared(-110LL, -10005LL, -10LL, -5LL) == 100000000+10000);
	}
	
	// rng_64
	// 乱数のため厳密なテストはできないが、可能な範囲で
	{
		const ll INF = 1LL<<61;
		vector<array<ll,2>> test = {{0, 99}, {INF-99, INF}};  // [lo,hi]
		for(auto &e : test)
		{
			for(int i = 0; i < 10000; i++)  // ざっと1万回回し、[lo,hi]の範囲が返ってくることのテスト
			{
				ll r = rng_64(e[0], e[1]);
				assert(e[0] <= r && r <= e[1]);
			}
			// 下限と上限が返ってくることのテスト (範囲は100なので、すぐ終わるでしょという考え方)
			while(true) { if(rng_64(e[0], e[1]) == e[0]) break; }
			while(true) { if(rng_64(e[0], e[1]) == e[1]) break; }
		}
		cout << "rng_64() test OK" << endl;
	}

	{
		vector<vector<int>> a(4, vector<int>(4)), b(4, vector<int>(4));
		a = {{1,0,0,0},
			{1,1,0,0},
			{1,0,2,0},
			{1,2,2,2}};
		b = {{1,1,1,1},
			{2,0,1,0},
			{2,2,0,0},
			{2,0,0,0}};
		rotate(a);
		assert(a == b);
	}

	{
		vector<vector<int>> a(2, vector<int>(3));  // a[2][3]
		vector<vector<int>> b(3, vector<int>(2));  // b[3][2]
		a = {{0,1,2},
			 {3,4,5}};
		b = {{3,0},
			 {4,1},
			 {5,2}};
		auto a_2 = rotate_2<int>(a);
		assert(a_2 == b);
		auto a_3 = rotate_2_rev<int>(a);
		a_3 = rotate_2_rev<int>(a_3);
		a_3 = rotate_2_rev<int>(a_3);
		assert(a_2 == a_3);  // 1回右回転と3回左回転は同一

		vector<vector<char>> c(1, vector<char>(4));
		vector<vector<char>> d(4, vector<char>(1));
		c = {{0,1,2,3}};
		d = {{0},
			 {1},
			 {2},
			 {3}};
		assert(rotate_2<char>(c) == d);
		auto c2 = rotate_2_rev<char>(c);
		c2 = rotate_2_rev<char>(c2);
		c2 = rotate_2_rev<char>(c2);
		assert(c2 == d);
	}

	{
		string s = "abCD";
		for(auto &e : s) e = revLowUp(e);
		assert(s == "ABcd");
	}

	{
		string s;
		s = "abcba";	assert( is_palindrome(s));  // s全体 奇数 
		s = "abba";		assert( is_palindrome(s));  // s全体 偶数
		s = "abcbz";	assert(!is_palindrome(s));  // s全体 奇数
		s = "abbz";		assert(!is_palindrome(s));  // s全体 偶数
		s = "a";		assert( is_palindrome(s));  // s全体 1文字
		s = "abcdczzyz";
		assert( is_palindrome(s, 2, 3));
		assert( is_palindrome(s, 5, 2));
		assert(!is_palindrome(s, 1, 3));
		assert(!is_palindrome(s, 5, 4));  // 1文字目は一致するが、2文字目に不一致
	}

	return 0;
}
