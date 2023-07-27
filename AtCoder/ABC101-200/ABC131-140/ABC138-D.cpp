#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題26
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc138

/*
 * 深さ優先探索で解く。
 * 
 * 各クエリについて、頂点pの子にそれぞれカウンターを足していくと、
 *   クエリがQ回
 *   クエリごとの木の操作がO(N)
 * となり、全体でO(QN)のためTLEとなる。
 * 
 * カウンターを足すのは自身および自身の子なので、
 * 深さ優先探索にて引数に「これまでのカウンターの総和」を入れておく。
 * 探索中の各頂点に対して、このカウンター総和と自身のカウンター値を足したものが、
 * その頂点のカウンター値となる。
 * 
 * 計算量はO(Q+N).
 */

using Graph = vector<vector<int>>;  // Graph[i]:頂点iと繋がっている頂点の一覧
Graph graph;
vector<int> counter;
vector<ll> answer;

// 頂点vから深さ優先探索を行う
// now_count:頂点vの親のカウンター値の総和
// parent:頂点vの親
void dfs(int v, int now_count, int parent)
{
	// 親のカウンター値の総和に、自身のカウンター値を足したものが、頂点vに対する答
	answer[v] = now_count + counter[v];

	for(auto &e : graph[v])  // e:vと繋がっている頂点
	{
		if(e == parent) continue;  // 親への逆流を防ぐ

		// 頂点e(親はv)から深さ優先探索
		dfs(e, answer[v], v);
	}
}

int main(void)
{
	// 1-indexed
	int i;
	int N, Q;
	cin >> N >> Q;
	graph.resize(N+1);
	for(i = 0; i < N-1; i++)  // 辺の数はN-1
	{
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}
	counter.resize(N+1);
	for(i = 0; i < Q; i++)
	{
		int p, x;
		cin >> p >> x;
		counter[p] += x;
	}

	answer.resize(N+1);
	// 根である頂点1から深さ優先探索をスタート
	dfs(1, 0, -1);

	for(i = 1; i <= N; i++)
	{
		cout << answer[i];
		if(i != N) cout << " ";
	}
	cout << endl;

	return 0;
}
