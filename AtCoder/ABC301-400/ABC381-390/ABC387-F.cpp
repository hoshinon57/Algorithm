#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC387 https://atcoder.jp/contests/abc387

/*
 * 自力で解けず、解説を見た。
 * SCC,DP,累積和などを用いて解く。ドチャクソ難しかった…
 * 
 * 基本は公式解説の通り。
 * https://atcoder.jp/contests/abc387/editorial/11834
 * 
 * 以下、0-indexedで記載する。
 * 1<=x<=Mも0<=x<=(M-1)に置き換える。
 * 
 * 入力に対し、Ai->iへ辺を張るグラフを考える。
 * これはサイクルと、サイクルから辺が出ているようなグラフとなり、
 * サイクルを1つの頂点とみなすと、そこを根とした木となる。
 * 
 * サイクル上の頂点はxの値が同一なので、SCCにて縮約する。
 * 以降、このグラフ(木)について考える。
 * 連結成分ごとに木になっている。
 * 
 * dp[i][j]を以下のように定義する。
 *   頂点i以下の部分木に限定して考えて、
 *   頂点iの値をjにしたときの、
 *   値の決め方。
 *   初期値は1.
 * 連結成分ごとに根をrとすると、
 *   Σ(j=0～(M-1))dp[r1][j] * Σdp[r2][j] * ...
 * が答。
 * 
 * 連結成分ごと、根からDFSしていけばよい。
 * DFSにて、ある頂点vにてdp[v][*]を求めるには、vから行ける頂点をeとして
 *   dp[v][j] = Σ(k=0～j)dp[e1][k] * Σdp[e2][k] * ...
 * となる。
 * これを単純に実装するとDFS部分がO(NM), 全体でO(N^2*M)とかになって間に合わないが、
 * 上記は累積和にてO(M)に改善でき、間に合う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・SCCにてサイクルを1つの頂点にまとめる。
 * ・テクニックとして、木上のDPにて子の情報を使いたい場合に
 *   まず子へ再帰して子のDP値を確定させ、
 *   親(呼び出し元)にて子のDP値を参照するという実装方法。
 *   ※再帰関数で戻り値を設定する必要は無い
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
	void build(Graph &g, vector<vector<int>> &grp, vector<int> &idx, Graph &shg)
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

#if 1  // 縮めたグラフを構築する場合、以下のようになる [verify:ABC245-F]
//		Graph shg;  // 呼び出し元から参照で渡す
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

// gの各頂点について入次数を計算し、返す (in[i]:頂点iの入次数)
vector<int> graph_in_deg(Graph &g) {
	int n = (int)g.size();
	vector<int> in(n);
	for(int i = 0; i < n; i++) {
		for(auto &e : g[i]) in[e]++;
	}
	return in;
}

int main(void)
{
	// 0-indexed
	ll i;
	ll N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		a--;
		g[a].push_back(i);  // Ai->iの辺
	}
	SCC scc;
	vector<vector<int>> grp;
	vector<int> idx;
	Graph shg;  // 縮約したグラフ
	scc.build(g, grp, idx, shg);
	auto in_deg = graph_in_deg(shg);

	vector<vector<mint>> dp(shg.size(), vector<mint>(M, 1));

	auto dfs = [&](auto &self, int v, int p) -> void
	{
		for(auto &e : shg[v])
		{
			if(e == p) continue;
			self(self, e, v);
#if 0  // 累積和を使わず、O(NM)の実装(TLE)
			for(int j = 0; j < M; j++)  // dp[v][j]
			{
				mint sum = 0;
				for(int k = 0; k <= j; k++)  // dp[e][k]
				{
					sum += dp[e][k];
				}
				dp[v][j] *= sum;
			}
#else  // 累積和にて高速化
			mint sum = 0;
			for(int k = 0; k < M; k++)
			{
				sum += dp[e][k];
				dp[v][k] *= sum;
			}
#endif
		}
	};
	mint ans = 1;
	for(i = 0; i < (int)shg.size(); i++)
	{
		if(in_deg[i] != 0) continue;
		// 以降、頂点iの入次数は0
		dfs(dfs, i, -1);
		mint sum = 0;
		for(int j = 0; j < M; j++)
		{
			sum += dp[i][j];
		}
		ans *= sum;
	}
	cout << ans.val() << endl;

	return 0;
}
