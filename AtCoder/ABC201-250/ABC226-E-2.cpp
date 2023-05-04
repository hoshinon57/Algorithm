#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC226 https://atcoder.jp/contests/abc226

/*
 * BFSで解く。
 * 
 * 考え方はABC226-E.cppと同じ。
 * 連結成分ごとの頂点数と辺数をカウントするのを、BFSにて実装した。
 */

using Graph = vector<vector<int>>;
Graph graph;
int N, M;
ll answer;

ll solve(void)
{
	answer = 1;
	vector<bool> seen(N, false);

	for(int i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 連結成分ごと
		seen[i] = true;

		// 頂点iからBFS
		// 連結成分の頂点数と辺数を求める
		ll v_count = 0;
		ll edge_count = 0;
		queue<int> que;
		que.push(i);
		while(!que.empty())
		{
			int v = que.front();
			que.pop();
			v_count++;  // vのぶん
			for(auto &e : graph[v])
			{
				edge_count++;  // v-e間を繋ぐ辺
				if(!seen[e])
				{
					seen[e] = true;
					que.push(e);
				}
			}
		}

		// 連結成分の頂点数と辺の数が等しいか
		// 辺は2回カウントしている
		if(v_count*2 == edge_count)
		{
			answer *= 2;
			answer %= MOD;
		}
		else
		{
			return 0;
		}
	}
	return answer;
}

int main(void)
{
	// 0-indexed
	int i;
	cin >> N >> M;
	graph.resize(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--; v--;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	cout << solve() << endl;

	return 0;
}
