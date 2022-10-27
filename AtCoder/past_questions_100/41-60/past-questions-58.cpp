#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題58
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2016yo/tasks/joi2016yo_e

/*
 * ヒープを用いたダイクストラ法で解く。
 * 
 * まず、各町が「ゾンビが支配する町からどれだけ離れているか」を幅優先探索で求める。
 * これをzombie_dist[]に設定する。
 * 
 * ダイクストラ法を使うにあたり、辺のコストは「行き先の町における宿泊費用」と考えることができる。
 * これを先ほどのzombie_dist[]を用いて計算する。
 * 
 * 最後にダイクストラ法で始点1からNへの最小コストを求めればよい。
 */

// ある頂点からの辺を表す構造体
struct Edge
{
	ll to;  // 辺の行き先
	ll w;  // 辺の重み
	Edge(ll to_, ll w_) : to(to_), w(w_) {}
};
using Graph = vector<vector<Edge>>;
Graph graph;

ll N, M, K, S;
ll P, Q;
vector<ll> zombie_no;  // ゾンビに支配されている町の番号
vector<ll> zombie_dist;  // zombie_dist[i]:i番目の町が、ゾンビからどれだけ離れているか

// 各町について、ゾンビが支配している町からどれだけ離れているかを計算し、zombie_dist[]に設定する
void bfs(void)
{
	queue<ll> que;  // 町の番号
	zombie_dist.assign(N+1, INF64);  // 1-indexed, 初期値は無限大に離れている
	for(auto &e : zombie_no)
	{
		zombie_dist[e] = 0;  // ゾンビのいる町は0
		que.push(e);  // ゾンビの支配する町から探索をスタート
	}

	// 各町の「ゾンビからどれだけ離れているか」を計算
	while(!que.empty())
	{
		ll v = que.front();
		que.pop();

		for(auto &e : graph[v])  // 町vから出る辺について探索
		{
			// dist[e.to] と dist[v]+1 の比較
			if(zombie_dist[e.to] > zombie_dist[v]+1)
			{
				zombie_dist[e.to] = zombie_dist[v]+1;
				que.push(e.to);
			}
		}
	}
}

// ダイクストラ法で、町1からNへの最小宿泊費を求める
ll dijkstra(void)
{
	// 町1スタート
	vector<ll> cost(N+1, INF64);  // cost[i]:町iへ行くまでの最小宿泊費
	cost[1] = 0;
	using pll = pair<ll,ll>;  // {宿泊費用、その町の番号}
	priority_queue<pll, vector<pll>, greater<pll>> que;
	que.push({cost[1], 1});

	while(!que.empty())
	{
		ll c = que.top().first;
		ll v = que.top().second;
		que.pop();

		if(c > cost[v]) continue;  // キューから取り出した値から、さらに更新済み

		for(auto &e : graph[v])  // 町vから出る辺について探索
		{
			if(cost[e.to] > cost[v] + e.w)
			{
				cost[e.to] = cost[v] + e.w;
				que.push({cost[e.to],e.to});
			}
		}
	}

	return cost[N];
}

int main(void)
{
	// 1-indexed
	ll i;
	cin >> N >> M >> K >> S;
	cin >> P >> Q;
	zombie_no.resize(K);
	for(i = 0; i < K; i++)  cin >> zombie_no[i];
	graph.resize(N+1);  // 1-indexed
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(Edge{b, 0});  // コストは後で計算するので0
		graph[b].push_back(Edge{a, 0});
	}

	bfs();

	// 辺のコストを設定する
	for(i = 0; i <= N; i++)
	{
		for(auto &e : graph[i])  // 町iから出る辺について探索
		{
			// i->e.to への移動について考えると、町e.toが危険かどうかで宿泊費用が変わる。
			// よってe.wに宿泊費用を設定すればよい。
			if(zombie_dist[e.to] == 0)
				e.w = INF64;  // ゾンビが支配している町へは行けない
			else if(zombie_dist[e.to] <= S)
				e.w = Q;  // 危険な町
			else
				e.w = P;

			if(e.to == N)  
				e.w = 0;  // 例外で、ゴールでは宿泊しない
		}
	}

	cout << dijkstra() << endl;

	return 0;
}
