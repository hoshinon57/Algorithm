#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ARC085 https://atcoder.jp/contests/arc085

/*
 * 自力で解けず、解説を見て実装した。
 * https://drken1215.hatenablog.com/entry/2019/12/17/113800
 * https://betrue12.hateblo.jp/entry/2019/07/05/012450
 * https://yosupo.hatenablog.com/entry/2015/03/31/134336
 * 
 * 最小カットを用いて解く。
 * いわゆる「燃やす埋める」や「プロジェクト選択」問題。
 * 
 * 最小カットに帰着するため、問題を以下のように置き換える。
 * (1)a[i]>0のとき
 *   無条件でa[i]円もらえて、
 *   割る場合はコストa[i], 割らない場合はコスト0
 * (2)a[i]<=0のとき
 *   無条件で0円もらえて、
 *   割る場合はコスト0, 割らない場合はコストa[i]
 * こうすると、無条件で得られる金額の総和を求めておき、
 * 最小カットでコストの最小値を求めれば、
 * 「無条件で得られる金額の総和」-「コスト最小値」が求める答になる。
 * 
 * a[1]～a[N]の頂点をまとめる頂点を2つ用意し、
 * 頂点0 -> a[1]～a[N] -> 頂点N+1 の方向へ辺を張る。
 * 頂点0からは割らない場合のコストを、
 * a[]から頂点N+1へは割る場合のコストを設定する。
 * 
 * xの倍数が書かれた宝石を全て割る、を実現するには、
 * 例えば頂点2->頂点4へ無限大の容量の辺を張る。
 * これにより、「宝石2を割り、宝石4は割らない」を防ぐことができる。
 * 
 * 余談として、事前に用意していたGraphおよびFordFulkersonは容量がint型であり、
 * long longに置き換える作業が必要であった。
 * メンバ変数だけでなく、関数の戻り値や引数、関数内のローカル変数、INF64など直す箇所が多く
 * WAの解消にかなり苦戦した。
 */

// グラフを表す構造体
struct Graph
{
	// 辺を表す構造体
	// rev:逆辺(to,from)がG[to]の中で何番目の要素か
	// cap:辺(from,to)の容量
	struct Edge
	{
		int rev, from, to;
		ll cap;
		Edge(int r, int f, int t, ll c) : rev(r), from(f), to(t), cap(c) {}
	};

	vector<vector<Edge>> list;

	// N:頂点数
	Graph(int N = 0) : list(N) {}

	// グラフの頂点数を返す
	size_t size(void)
	{
		return list.size();
	}

	// GraphのインスタンスをGとして、G.list[v]をG[v]と書けるようにしておく
	vector<Edge>& operator [] (int v)
	{
		return list[v];
	}

	// 辺e=(u,v)の逆辺e'=(v,u)を取得する
	Edge& rev_edge(const Edge &e)
	{
		return list[e.to][e.rev];
	}

	// 辺e=(u,v)に流量fのフローを流す
	// eの流量がf減少し、逆辺e'=(v,u)の流量はf増加する
	void run_flow(Edge &e, ll f)
	{
		e.cap -= f;
		rev_edge(e).cap += f;
	}

	// 頂点fromからtoへ、容量capの辺を張る
	// このとき逆方向to->fromへも容量0の辺を張っておく
	void add_edge(int from, int to, ll cap)
	{
		int from_size = (int)list[from].size();
		int to_size = (int)list[to].size();
		list[from].push_back(Edge(to_size, from, to, cap));
		list[to].push_back(Edge(from_size, to, from, 0));
	}
};

struct FordFulkerson
{
	vector<bool> seen;

	FordFulkerson() {}

	// 深さ優先探索を用いて、残余グラフ上でs-tパスを見つける
	// 戻り値：s-tパス上の容量の最小値 パスが見つからなかった場合は0
	// v:探索中の頂点, t:t, f:探索過程の各辺の容量の最小値
	ll ff_dfs(Graph &G, int v, int t, ll f)
	{
		// 終端に到達したら戻る
		if(v == t) return f;

		seen[v] = true;
		for(auto &e : G[v])  // 頂点vから出ている辺について探索
		{
			if(seen[e.to]) continue;  // 探索済みならスルー
			if(e.cap == 0) continue;  // 容量が0、つまり辺として存在しない

			// vをe.toに更新して、再帰
			// fはe.capとの小さい方を採用する
			// s-tパスが見つからなかった場合、戻り値は0
			ll flow = ff_dfs(G, e.to, t, min(f, e.cap));

			if(flow == 0) continue;  // e.to経由ではs-tパスは見つからなかった

			// 辺eに容量flowのフローを流す
			G.run_flow(e, flow);

			// s-tパスを見つけた場合、パス上の最小容量を返す
			return flow;
		}

		return 0;  // s-tパスが見つからなかった
	}

	// グラフGのs-t間の最大流量を求める
	// ただし関数内でGは残余グラフに変化する
	ll solve(Graph &G, int s, int t)
	{
		ll res = 0;
		
		// 残余グラフにs-tパスが無くなるまで繰り返す
		while(true)
		{
			seen.assign((int)G.size(), false);

			int f = ff_dfs(G, s, t, INF64);
			if(f == 0) break;  // s-tパスが見つからなかったら終了
			res += f;
		}

		return res;
	}
};

int main(void)
{
	int i, j;
	int N;
	cin >> N;
	ll baseGain = 0;  // 問題をコストに置き換えたときに、「無条件でもらえる金額」の総和
	vector<ll> a(N+1);  // 1-indexed
	Graph g(N+2);  // 頂点をまとめる頂点を2つ用意する
	// 頂点0 -> a[1]～a[N] -> 頂点N+1 の方向へ辺を張る
	// 頂点0からは割らない場合のコスト、a[]から頂点N+1へは割る場合のコストを設定する
	for(i = 1; i <= N; i++)
	{
		cin >> a[i];
		if(a[i] > 0)  // 割らなかったら、a[i]円もらえる
		{
			baseGain += a[i];
			g.add_edge(0, i, 0);  // 割らないことによるコストは0
			g.add_edge(i, N+1, a[i]);  // 割ることによるコストはa[i]
		}
		else  // 割らなかったら、a[i]円払う
		{
			// baseGain += 0;
			g.add_edge(0, i, -a[i]);  // 割らないことによるコストは -a[i]
			g.add_edge(i, N+1, 0);  // 割ることによるコストは0
		}
	}
	for(i = 1; i <= N; i++)
	{
		for(j = i+i; j <= N; j+=i)
		{
			// a[i]を割るなら、a[j]も割らないといけない
			g.add_edge(i, j, INF64);
		}
	}

	FordFulkerson ff;
	int s = 0;
	int t = N+1;
	// 無条件でもらえる金額 - コストの最小値 が答となる
	cout << baseGain - ff.solve(g, s, t) << endl;

	return 0;
}
