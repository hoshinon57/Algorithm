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
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

/*
 * その他のライブラリ
 * ・床関数,天井関数 floor_div, ceil_div
 * ・二分探索でidxと要素数を返す bi_idxnum_miman, 他
 * ・二分探索で位置st以降でnum個目の登場位置(の次の位置)を返す bi_nextpos
 * ・bビット目の確認、ビットON/OFF isbiton, setbit, unbit
 * ・桁数を返す cal_digit
 * ・p^0～p^(n-1)を事前計算 cal_pow_inadv
 * ・k乗根 kth_root_integer
 * ・2点の距離の2乗 dist_squared
 * ・非負mod smod
 * ・乱数生成 rng_64
 * ・N*N配列を回転 rotate
 * ・Y*X配列を回転 rotate_2, rotate_2_rev
 * ・nをbase進法で表したときの値 chg_base
 * ・b進法で表される文字列sを、10進数へ変換 chg_base_to10
 * ・配列を1つの値にエンコード/デコード enc_VecToNum, dec_ValToVec
 * ・ランレングス圧縮 rle
 * ・2つのsetをマージ(マージテク使用) set_merge
 * ・大文字小文字を反転 revLowUp
 * ・extgcd
 * ・SCC
 * ・円環上の距離や公差判定 move_on_ring
 * ・3x3のマス目について縦/横/斜めがそろったかを判定 -> ABC349-E.cppを参照
 * ・文字列が回文かを判定 is_palindrome
 * ・区間をsetで管理 interval_set
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

// 要素の登場位置を列挙しているposについて、位置st以降でnum個目の登場位置の"次の位置"を返す (num個なければINF)
// ⇒よって戻り値は"次に見るべき位置"となる
// 登場位置や個数を扱うので、型はいずれもintにしている
// [使用例]ABC381-E, ABC381-F
int bi_nextpos(vector<int> &pos_, int st, int num) {
	if(num == 0) return st;  // 1個も取らないならstから変わらず
	auto idx = lower_bound(pos_.begin(), pos_.end(), st) - pos_.begin();  // 1個目
	idx = idx + num - 1;  // num個目の登場位置
	return ((idx < (int)pos_.size()) ? pos_[idx]+1 : INF32);
};

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

// n%mを計算し、非負つまり[0,m)の範囲で返す
// 参考：https://atcoder.jp/contests/abc376/submissions/58995270 (ecottea氏のテンプレート)
template<typename T>
T smod(T n, T m) {
	n %= m;
	if(n < 0) n += m;
	return n;
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

// [verify]ABC220-B
// b進法で表される文字列sを、10進数にして返す (b<=10)
// 例)chg("120",3)=15, chg("120",10)=120
ll chg_base_to10(string &s, ll b) {
	ll ret = 0;
	for(auto &c : s) {
		ret *= b;
		ret += c - '0';
	}
	return ret;
}

// vector<int>の配列を1つの値にエンコード/デコードする関数
// 配列の値が[0,base), 要素数がpのとき、組み合わせの状態数は base^p となる。これを1つの値にエンコードする
// エンコードした値をDPの添え字に使う想定
// ★★ENC_VECTONUM_BASE_DEFAULTに値を設定すること★★
// [verify]ABC380-F,ABC322-E
const int ENC_VECTONUM_BASE_DEFAULT = 0;	// enc_VecToNum(), dec_ValToVec()で指定する基数のデフォルト 問題内で基数が変わることも無さそうなので、これで指定すれば呼び出し時の指定を省ける
// 要素の値が[0,base)の範囲であるstateについて、[0]から順にbase進法としてみなして1つの整数値にまとめる(変換する)
// 例：state={0,3,2,1}, base=4であれば、0*4^0+3*4^1+2*4^2+1*4^3 を返す
// 戻り値をDPの添え字に使うことを想定しているため、int型
int enc_VecToNum(vector<int> &state, int base = ENC_VECTONUM_BASE_DEFAULT) {
	int ans = 0, p = 1;
	for(int i = 0; i < (int)state.size(); i++) {
		ans += p*state[i];
		p *= base;
	}
	return ans;
}
// enc_VecToNum()での戻り値からvector<int>に変換して返す
// siz:vector<int>の要素数
vector<int> dec_ValToVec(int n, int siz, int base = ENC_VECTONUM_BASE_DEFAULT) {
	vector<int> ret(siz);
	for(int i = 0; i < siz; i++) {
		ret[i] = n % base;
		n /= base;
	}
	return ret;
}

// 文字列strをランレングス圧縮して {文字,長さ} のpairの列挙で返す
// [補足]配列に対して扱いたい場合、引数をstring->vectorに変えるだけで良い
//       "長さ"は呼び出し元で乗算する可能性があるため、オーバーフロー防止でll型としている (ABC369-C)
// auto enc = rle<char>(s); といった形で呼び出す
// for(auto [c,len] : enc) が使い方の一例
// [参考]https://algo-logic.info/run-length/  [verify]ABC019-B,ABC380-B
template <typename T> vector<pair<T,ll>> rle(string &str) {
	vector<pair<T,ll>> ret;
	int n = str.size();
	for(int l = 0, r = 0; l < n; ) {  // 尺取り法っぽく [l,r)が条件を満たす
		while(r < n && str[l] == str[r]) r++;
		ret.push_back({str[l], r-l});
		l = r;
	}
	return ret;
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
 *   ABC296-E(Transition Game) evima氏の解説
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

/*
 * 円環上の距離や移動、公差判定のライブラリ
 * [verify]ABC376-B, ABC376-F
 * [使い方]
 * コンストラクタ、またはset_n()にて1周の長さを指定する。
 *   move_on_ring ring(N);
 *   or ring.set_n(N);
 * 
 * 座標は 0,1,2,...,n-1 の並びで、n-1の次は0とする。
 * (動作には影響しないが、座標は時計回りで1ずつ増え、反時計回りで1ずつ減るイメージ)
 * 参考：https://atcoder.jp/contests/abc376/tasks/abc376_b
 * 
 * 方向指定の関数は DIR_PLUS_CLOCK, DIR_MINUS_REVCLOCKを指定する。
 * 
 * 座標は[0,n)の範囲で指定すること。
 * ※範囲外になりそうな場合、呼び出し元で nの剰余を求めて負ならnを加算 でいける、はず。
 * 
 * [参考]
 * https://x.com/eco4kb/status/1847677647482245472
 * https://qiita.com/NokonoKotlin/items/f2cf76a7abf19b6b67eb
 */
template <typename T>
class move_on_ring {
	T n;  // 1周の長さ
public:
	const int DIR_PLUS_CLOCK = 1;  // 方向指定 座標プラス方向 (時計回り)
	const int DIR_MINUS_REVCLOCK = -1;  // 方向指定 座標マイナス方向 (反時計回り)
	move_on_ring(T n_) : n(n_) {}
	move_on_ring(void) : n(0) {}
	void set_n(T n_) { n = n_; }
	// s,t間の距離を返す (時計回りと反時計回りとで短い方)
	T dist(T s, T t) {
		if(s == t) return (T)0;
		if(s > t) swap(s, t);  // s<=tにする 対称性よりswapして良い
		return min(t-s, n-(t-s));
	}
	// s,t間の距離を返す (s->tへdir方向へ移動したときの距離)
	T dist_dir(T s, T t, int dir) {
		if(s == t) return (T)0;  // n-(t-s)==n の対策
		if(s > t) {  // s<=tにする その場合dirも反転
			swap(s, t);
			dir = ((dir == DIR_PLUS_CLOCK) ? (DIR_MINUS_REVCLOCK) : (DIR_PLUS_CLOCK));
		}
		if(dir == DIR_PLUS_CLOCK) return t-s;
		else return n-(t-s);
	}
	// sからdir方向へiだけ移動した後の座標を返す
	T move(T s, T i, int dir) {
		i %= n;
		if(dir == DIR_PLUS_CLOCK) s = (s+i)%n;
		else s = (s-i+n)%n;
		return s;
	}
	// s->tへdir方向で移動するとき、途中でxを通るならtrue (x=s,tのときもtrue, つまり両端含む)
	bool cross(T s, T t, T x, int dir) {
		return dist_dir(s, x, dir) <= dist_dir(s, t, dir);
	}
};

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

/*
 * 区間をsetで管理する構造体 閉区間で表すことに注意
 * llとINF64を定義しておくこと
 * メインは(1)(2)で、問題によっては(3)以降を使う感じか
 * またはset構造体のsを直接操作する
 * [verify]
 *   ABC364-F, "CODE FESTIVAL 2015 予選B"-D,
 *   AOJ 2880
 * 
 * コンストラクタ : interval_set s; or interval_set s(l,r);  後者はl<=i<=rを満たす[i,i]を区間登録
 * (1)get(pos) : 値posが含まれる区間のイテレータを返す。該当区間がなければs.end()を返す。
 * (2)ins(l,r) : 閉区間[l,r]を区間に追加する。その際に重なる区間は統合される。戻り値は区間数。
 * (3)same(x,y) : x,yが同一区間か判定
 * (4)merge(pos) : 位置posを指定して、隣接した区間が両隣にあれば統合する
 *    例){1,3},{4,6},{7,9}でmerge(4);で呼び出すと{1,9}になる
 *    マス目を埋めていくような問題で使えるかも
 * (5-1)get2_next(pos) : 値posが区間に含まれていればそこを、無ければ次の区間のイテレータを返す
 * (5-2)get2_prev(pos) : 同じく手前の区間のイテレータを返す 手前が無ければ該当無しとしてendを返す
 */
struct interval_set {
	using pll = pair<ll, ll>;
	set<pll> s;
	interval_set() {}
	interval_set(ll st, ll ed) { init(st, ed); }
	// チェック無しでst<=i<=edを満たす[i,i]を区間登録
	void init(ll st, ll ed) {
		for(ll i = st; i <= ed; i++) {
			s.insert({i, i});
		}
	}
	// 値posが含まれる区間のイテレータを返す
	// 該当区間がなければs.end()を返す
	set<pll>::iterator get(ll pos) {
		if(s.size() == 0) return s.end();
		// upper_boundの1つ手前をチェック
		auto itr = s.upper_bound({pos, INF64});
		if(itr == s.begin()) return s.end();
		itr--;
		if(itr->first <= pos && pos <= itr->second) return itr;  // left<=pos<=right
		else return s.end();
	}
	// 値posが区間に含まれていればそこを、無ければ次の区間のイテレータを返す
	set<pll>::iterator get2_next(ll pos) {
		if(s.size() == 0) return s.end();
		auto itr = get(pos);
		if(itr != s.end()) return itr;  // 区間があればそこを返す
		else return s.upper_bound({pos, INF64});  // 無い場合、次のイテレータ
	}
	// get2_nextと同じく手前の区間のイテレータを返す 手前が無ければ該当無しとしてendを返す
	set<pll>::iterator get2_prev(ll pos) {
		if(s.size() == 0) return s.end();
		auto itr = get(pos);
		if(itr != s.end()) return itr;  // 区間があればそこを返す
		// 無い場合は手前のイテレータ ただしbeginなら該当無しとしてendを返す
		if( (itr = s.upper_bound({pos, INF64})) == s.begin()) return s.end();
		else return prev(itr);
	}
	// 閉区間[l,r]を区間に追加し、その際に重なる区間は統合する
	size_t ins(ll l, ll r) {
		ll st, ed;  // [l,r]を囲む区間[st,ed]
		auto itr = get(l);
		if(itr == s.end()) {  // lが区間に存在しない
			st = l;
			s.insert({st, st});  // 後述の実装を簡易にするため区間登録しておく
		}
		else st = itr->first;

		if( (itr = get(r)) == s.end()) ed = r;
		else ed = itr->second;

		// 先に重なる区間を消してから、統合された区間を登録
		itr = get(st);
		while(itr != s.end() && itr->first <= ed) {
			itr = s.erase(itr);
		}
		s.insert({st, ed});
		return s.size();
	}
	// x,yが同一区間か判定
	bool same(ll x, ll y) {
		auto itr = get(x);
		if(itr == s.end()) return false;  // xが区間に含まれていないならfalse
		return (itr->first <= y && y <= itr->second);  // yがxの区間内か
	}
	// 位置posを指定して、隣接した区間が両隣にあれば統合する
	// 例：{1,3},{4,6},{7,9} で merge(4); で呼び出せば {1,9} となる
	// マス目を埋めていくような問題で使えるかも
	// posが区間に含まれないときの動作は未定義
	void merge(ll pos) {
		auto itr = get(pos);
		auto nxt = next(itr);
		if(nxt != s.end()) {
			if(itr->second+1 == nxt->first) ins(itr->second, nxt->first);
		}
		itr = get(pos);
		if(itr != s.begin()) {
			auto prv = prev(itr);
			if(prv->second+1 == itr->first) ins(prv->second, itr->first);
		}
	}
	void debug(void) {
		// return;
		for(auto &e : s) {
			cout << "[" << e.first << "," << e.second << "], ";
		}
		cout << endl;
	}
};

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

	{
		vector<int> pos = {2, 5, 6, 7, 10, 12};  // 要素の登場位置を列挙している、と考える
		assert(bi_nextpos(pos, 1, 4) == 8);  // 1以降で要素が4つ登場したときの、次の位置
		assert(bi_nextpos(pos, 5, 2) == 7);
		assert(bi_nextpos(pos, 10, 3) == INF32);  // 10以降に要素3つは存在しない
		assert(bi_nextpos(pos, 12, 1) == 13);
		assert(bi_nextpos(pos, 13, 1) == INF32);
		assert(bi_nextpos(pos, 0, 6) == 13);
		assert(bi_nextpos(pos, 0, 7) == INF32);
		assert(bi_nextpos(pos, 4, 0) == 4);  // 取る個数が0
		assert(bi_nextpos(pos, 5, 0) == 5);
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

	// smod
	{
		assert(smod(5, 3) == 2);
		assert(smod(-2, 3) == 1);
		assert(smod(-3, 3) == 0);
		assert(smod(-4, 3) == 2);
		assert(smod(-1000, 222) == 110);
		assert(smod(0, 3) == 0);
		assert(smod(3, 3) == 0);
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
		vector<int> v = {3, 2, 1, 0, 2};
		int n = enc_VecToNum(v, 4);  // 3*4^0+2*4^1+1*4^2+0*4^3+2*4^4
		assert(n == 539);
		auto v2 = dec_ValToVec(n, 5, 4);
		assert(v == v2);
	}

	{
		{
			string s = "aaabccc00011";
			vector<pair<char,ll>> a = {{'a',3}, {'b',1}, {'c',3}, {'0',3}, {'1',2}};
			auto enc = rle<char>(s);
			assert(enc == a);
		}
		{
			string s = "0";
			vector<pair<char,ll>> a = {{'0',1}};
			auto enc = rle<char>(s);
			assert(enc == a);
		}
		{
			string s = "01";
			vector<pair<char,ll>> a = {{'0',1}, {'1',1}};
			auto enc = rle<char>(s);
			assert(enc == a);
		}
	}

	{
		string s = "abCD";
		for(auto &e : s) e = revLowUp(e);
		assert(s == "ABcd");
	}

	{
		int N = 6;
		move_on_ring r(N);
		assert(r.dist(0, 2) == 2);
		assert(r.dist(0, 5) == 1);
		assert(r.dist(0, 0) == 0);
		assert(r.dist(1, 3) == 2);
		assert(r.dist(1, 5) == 2);
		assert(r.dist(1, 1) == 0);
		assert(r.dist_dir(0, 2, r.DIR_PLUS_CLOCK) == 2);
		assert(r.dist_dir(0, 2, r.DIR_MINUS_REVCLOCK) == 4);
		assert(r.dist_dir(0, 0, r.DIR_PLUS_CLOCK) == 0);
		assert(r.dist_dir(0, 0, r.DIR_MINUS_REVCLOCK) == 0);
		assert(r.dist_dir(2, 2, r.DIR_PLUS_CLOCK) == 0);
		assert(r.dist_dir(2, 1, r.DIR_PLUS_CLOCK) == 5);
		assert(r.dist_dir(2, 2, r.DIR_MINUS_REVCLOCK) == 0);
		assert(r.dist_dir(2, 3, r.DIR_MINUS_REVCLOCK) == 5);
		assert(r.move(0, 2, r.DIR_PLUS_CLOCK) == 2);
		assert(r.move(0, 2, r.DIR_MINUS_REVCLOCK) == 4);
		assert(r.move(0, 0, r.DIR_PLUS_CLOCK) == 0);
		assert(r.move(0, 100, r.DIR_PLUS_CLOCK) == 4);
		assert(r.move(5, 3, r.DIR_PLUS_CLOCK) == 2);
		assert(r.move(1, 3, r.DIR_MINUS_REVCLOCK) == 4);
		assert( r.cross(0, 3, 1, r.DIR_PLUS_CLOCK));
		assert(!r.cross(0, 3, 4, r.DIR_PLUS_CLOCK));
		assert( r.cross(0, 3, 4, r.DIR_MINUS_REVCLOCK));
		assert(!r.cross(0, 3, 1, r.DIR_MINUS_REVCLOCK));
		assert( r.cross(0, 3, 0, r.DIR_PLUS_CLOCK));
		assert( r.cross(0, 3, 0, r.DIR_MINUS_REVCLOCK));
		assert( r.cross(0, 3, 3, r.DIR_PLUS_CLOCK));
		assert( r.cross(0, 3, 3, r.DIR_MINUS_REVCLOCK));
		assert( r.cross(1, 5, 2, r.DIR_PLUS_CLOCK));
		assert( r.cross(1, 5, 0, r.DIR_MINUS_REVCLOCK));
		assert(!r.cross(1, 5, 0, r.DIR_PLUS_CLOCK));
		assert(!r.cross(1, 5, 2, r.DIR_MINUS_REVCLOCK));
		assert( r.cross(1, 5, 1, r.DIR_PLUS_CLOCK));
		assert( r.cross(1, 5, 5, r.DIR_PLUS_CLOCK));
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

	{
		{
			interval_set s(0, 99);  // [0,0]から[99,99]まで区間登録
			assert(s.get(100) == s.s.end());
			assert(s.get(-1) == s.s.end());
			assert(s.s.size() == 100);
		}
		{
			interval_set s;
			s.ins(0, 3); s.ins(4, 6); s.ins(1, 5);  // {0, 6};
			assert(s.s.begin()->first == 0 && s.s.begin()->second == 6 && s.s.size() == 1);
			s.ins(9, 12); s.ins(9, 15);  // {0, 6}, {9, 15}
			assert(prev(s.s.end())->first == 9 && prev(s.s.end())->second == 15 && s.s.size() == 2);
			s.ins(-4, 16);  // {-4, 16}
			assert(s.s.begin()->first == -4 && s.s.begin()->second == 16 && s.s.size() == 1);
			s.s.clear(); s.ins(1, 5); s.ins(7, 10); s.ins(6, 7);  // {1,5},{6,10}
			assert(s.s.begin()->first == 1 && s.s.begin()->second == 5 && s.s.size() == 2);
			s.s.clear(); s.ins(1, 5); s.ins(7, 10); s.ins(5, 6);  // {1,6},{7,10}
			assert(s.s.begin()->first == 1 && s.s.begin()->second == 6 && s.s.size() == 2);
		}
		{
			interval_set s;
			s.ins(1, 3); s.ins(5, 7);
			assert( s.same(1, 1)); assert( s.same(1, 3)); assert( s.same(3, 1));
			assert(!s.same(3, 5));
			assert(!s.same(3, 0)); assert(!s.same(3, 4)); assert(!s.same(8, 8));  // 片方もしくは両方が区間に含まれていない
		}
		{
			interval_set s;
			s.ins(4, 6); s.merge(4);  // {4,6}
			assert(s.s.begin()->first == 4 && s.s.begin()->second == 6 && s.s.size() == 1);
			s.ins(1, 3); s.ins(7, 9); s.merge(4);  // {1,3},{4,6},{7,9} -> merge -> {1,9}
			assert(s.s.begin()->first == 1 && s.s.begin()->second == 9 && s.s.size() == 1);
		}
		{
			interval_set s;
			s.ins(4, 6); s.ins(8, 10);
			auto itr = s.get2_next(4);	assert(itr->first == 4 && itr->second == 6);
			itr = s.get2_next(6);	assert(itr->first == 4 && itr->second == 6);
			itr = s.get2_next(11);	assert(itr == s.s.end());
			itr = s.get2_next(3);	assert(itr->first == 4 && itr->second == 6);
			itr = s.get2_next(7);	assert(itr->first == 8 && itr->second == 10);

			s.s.clear();
			s.ins(4, 6); s.ins(8, 10);
			itr = s.get2_prev(4);	assert(itr->first == 4 && itr->second == 6);
			itr = s.get2_prev(6);	assert(itr->first == 4 && itr->second == 6);
			itr = s.get2_prev(3);	assert(itr == s.s.end());
			itr = s.get2_prev(7);	assert(itr->first == 4 && itr->second == 6);
			itr = s.get2_prev(11);	assert(itr->first == 8 && itr->second == 10);
		}
	}

	return 0;
}
