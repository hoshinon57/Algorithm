#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>  // numeric_limits
#include <cassert>
using namespace std;
typedef long long ll;

/*
 * その他のライブラリ
 * ・床関数,天井関数
 * ・bビット目の確認、ビットON/OFF
 * ・N*N配列を回転
 * ・nをbase進法で表したときの値
 * ・extgcd
 * ・SCC
 * ・3x3のマス目について縦/横/斜めがそろったかを判定
 */

// a/b以下の最大の整数(床関数) floor(5,2)=2, floor(-5,2)=-3
template <typename T> T floor_div(T a, T b){ if(b<0){a=-a; b=-b;} if(a>0){return a/b;} else {return (a-b+1)/b;}}
// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

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

// [verify]ABC336-C, ABC234-C
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

#if 0  // 縮めたグラフを構築する場合、以下のようになる模様 (動作確認はほとんどできていない)
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

	return 0;
}
