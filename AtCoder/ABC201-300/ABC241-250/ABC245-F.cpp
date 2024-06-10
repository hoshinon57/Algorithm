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

// ABC245 https://atcoder.jp/contests/abc245

/*
 * 強連結成分分解(SCC)で解く。
 * kyopro_friends氏の解説とほぼ同じ。
 *   https://atcoder.jp/contests/abc245/editorial/3664
 * 
 * グラフをSCCしたのち、その各グループをグラフ化したものを考える。 ★これはDAGになる。
 * グループに含まれる頂点数が2以上のとき、その頂点らは題意を満たす。
 * また、そのグループにたどりつけるグループについても、題意を満たす。
 * 
 * よって、グラフ(グループ)の頂点ごとに重みwを設定し、グループ内の頂点数としておく。
 * グループからたどれるwの最大値を求めて、これが2以上かどうかを求める。
 * 自分はこれをメモ化DFSで実装した。
 * 
 * [ACまでの思考の流れ]
 * ・「移動を繰り返すことができる」はループになってそう。
 * ・ちょうどSCCを学んだ時期だったので、SCCを思いついた。
 *   SCCのグループから作ったグラフを考えたときに、
 *   グループ内の頂点数が2以上のものがあれば、そこはループになっている。
 *   そのグループ自体、およびそのグループへ行けるグループは、題意を満たしそう。
 * ・SCCのグループでグラフ化したものはDAGになる、というのはしっかり覚えておく。
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

#if 1  // 縮めたグラフを構築する場合、以下のようになる模様 (動作確認はほとんどできていない)
		// Graph shg;  // 呼び出し元から参照で渡す
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

// shgにて、頂点vからたどれる頂点のうち、最大のwを求めてwmaxに設定する
int dfs(Graph &shg, vector<int> &w, vector<int> &wmax, int v)
{
	if(wmax[v] != 0) return wmax[v];  // メモ化済み
	wmax[v] = w[v];
	for(auto &e : shg[v])
	{
		chmax(wmax[v], dfs(shg, w,wmax, e));
	}
	return wmax[v];
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	Graph shg;  // SCCで縮めたDAG
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
	}
 	SCC scc;
 	vector<vector<int>> grp;
 	vector<int> idx;
	scc.build(g, grp, idx, shg);

	vector<int> w;  // w[i]:縮めたDAGにて、頂点iの重み(=そのグループの頂点数)
	for(auto &e : grp) w.push_back(e.size());
	vector<int> wmax(grp.size());
	int ans = 0;
	for(i = 0; i < (int)shg.size(); i++)
	{
		dfs(shg, w, wmax, i);  // 頂点iからDFS
		if(wmax[i] > 1)
		{
			ans += w[i];
		}
	}
	cout << ans << endl;

	return 0;
}
